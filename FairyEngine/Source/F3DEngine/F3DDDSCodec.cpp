/*
 * ------------------------------------------------------------------------
 *  Name:   F3DDDSCodec.cpp
 *  Desc:   本文件为引擎实现了一个DDS图像解码器。
 *  Author: Yish
 *  Date:   2010/11/16
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. 2010 All right Observed.
 * ------------------------------------------------------------------------
 */


#include "F3DPCH.h"

#include "F3DDDSCodec.h"
#include "F3DPixelUtil.h"


///////////////////////////////////////////////////////////////////////////
// DDS格式的一些常量和结构体

// 像素格式
struct SDDSPixelFormat
{
    ulong dwSize;
    ulong dwFlags;
    ulong dwFourCC;
    ulong dwRGBBitCount;
    ulong dwRBitMask;
    ulong dwGBitMask;
    ulong dwBBitMask;
    ulong dwRGBAlphaBitMask;
};

enum {
    DDPF_ALPHAPIXELS = 0x00000001L,
    DDPF_ALPHA       = 0x00000002L,
    DDPF_FOURCC      = 0x00000004L,
    DDPF_RGB         = 0x00000040L
};

struct SDDSCaps2
{
    ulong dwCaps1;
    ulong dwCaps2;
    ulong dwReserved[2];
};

enum {
    DDSCAPS_ALPHA   = 0x00000002L,
    DDSCAPS_COMPLEX = 0x00000008L,
    DDSCAPS_TEXTURE = 0x00001000L,
    DDSCAPS_MIPMAP  = 0x00400000L
};

enum {
    DDSCAPS2_CUBEMAP            = 0x00000200L,
    DDSCAPS2_CUBEMAP_POSITIVEX  = 0x00000400L,
    DDSCAPS2_CUBEMAP_NEGATIVEX  = 0x00000800L,
    DDSCAPS2_CUBEMAP_POSITIVEY  = 0x00001000L,
    DDSCAPS2_CUBEMAP_NEGATIVEY  = 0x00002000L,
    DDSCAPS2_CUBEMAP_POSITIVEZ  = 0x00004000L,
    DDSCAPS2_CUBEMAP_NEGATIVEZ  = 0x00008000L,
    DDSCAPS2_VOLUME             = 0x00200000L
};

struct SDDSSurfaceDesc2
{
    ulong dwSize;
    ulong dwFlags;
    ulong dwHeight;
    ulong dwWidth;
    ulong dwPitchOrLinearSize;
    ulong dwDepth;
    ulong dwMipMapCount;
    ulong dwReserved1[11];
    SDDSPixelFormat ddpfPixelFormat;
    SDDSCaps2 ddsCaps;
    ulong dwReserved2;
};

enum {
    DDSD_CAPS           = 0x00000001L,
    DDSD_HEIGHT         = 0x00000002L,
    DDSD_WIDTH          = 0x00000004L,
    DDSD_PITCH          = 0x00000008L,
    DDSD_ALPHABITDEPTH  = 0x00000080L,
    DDSD_PIXELFORMAT    = 0x00001000L,
    DDSD_MIPMAPCOUNT    = 0x00020000L,
    DDSD_LINEARSIZE     = 0x00080000L,
    DDSD_DEPTH          = 0x00800000L
};

struct SDDSHeader
{
    ulong dwMagic;    // ('D','D','S',' ')
    SDDSSurfaceDesc2 surfaceDesc;
};

#define FOURCC_DXT1	F_MAKEFOURCC( 'D','X','T','1' )
#define FOURCC_DXT2	F_MAKEFOURCC( 'D','X','T','2' )
#define FOURCC_DXT3	F_MAKEFOURCC( 'D','X','T','3' )
#define FOURCC_DXT4	F_MAKEFOURCC( 'D','X','T','4' )
#define FOURCC_DXT5	F_MAKEFOURCC( 'D','X','T','5' )

///////////////////////////////////////////////////////////////////////////

struct Color8888
{
    uchar b,g,r,a;
};

struct Color565
{
    ushort b : 5;
    ushort g : 6;
    ushort r : 5;
};

struct DXTColBlock
{
    Color565 colors[2];
    uchar row[4];
};

struct DXTAlphaBlockExplicit
{
    ushort row[4];
};

struct DXTAlphaBlock3BitLinear
{
    uchar alpha[2];
    uchar data[6];
};

struct DXT1Block
{
    DXTColBlock color;
};

struct DXT3Block    // 也用于DXT2
{
    DXTAlphaBlockExplicit alpha;
    DXTColBlock color;
};

struct DXT5Block    // 也用于DXT4
{
    DXTAlphaBlock3BitLinear alpha;
    DXTColBlock color;
};

// 定义一些加载DDS的辅助函数

// 获取一个Block的颜色值
static void GetBlockColors( const DXTColBlock& block,Color8888 colors[4],bool isDXT1 );

// 对DXT块进行解码
template <typename DECODER>
static void DecodeDXTBlock( uchar* destData,const uchar* srcBlock,size_t width,size_t height,size_t pitch );

// 加载普通DDS格式
static F3DImage* LoadRGBImage( FVFile* pFile,const SDDSSurfaceDesc2& desc );

// 加载DXT压缩格式
static F3DImage* LoadDXTImage( int DXT,FVFile* pFile,const SDDSSurfaceDesc2& desc );

// 加载指定DXT格式的辅助函数
template <typename DECODER>
static void LoadDXTHelper( FVFile* pFile,F3DImage* image,size_t width,size_t height,size_t pitch );

///////////////////////////////////////////////////////////////////////////

/** 构造函数
*/
F3DDDSCodec::F3DDDSCodec(void)
{
    m_sCodecName = "DDS";
}

/** 析构函数
*/
F3DDDSCodec::~F3DDDSCodec(void)
{
}

/** 加载图像文件
@Param 图像文件的路径
*/
F3DImage* F3DDDSCodec::Load( const char* filename )
{
	FVFile file;
	if( !file.Open(filename,FVFile::VFILE_OPENEXIST) )
	{
		FLOG_WARNINGF( "F3DDDSCodec::Load, Open the image file (%s) failed!",filename );
		return NULL;
	}

    return LoadImageFromVFile( &file );
}

/** 从虚拟文件流中加载图像数据
@Param 虚拟文件流的指针
*/
F3DImage* F3DDDSCodec::LoadImageFromVFile( FVFile* pFile )
{
    F3DImage* image;

    SDDSHeader header;
    pFile->Read( &header,sizeof(SDDSHeader) );

    if( header.dwMagic != 0x20534444 )
    {
		FLOG_WARNING( "F3DDDSCodec::LoadImageFromVFile, Invalid DDS file!" );
		return NULL;
    }

    // 暂不支持cube和volume纹理
    if( header.surfaceDesc.ddsCaps.dwCaps2 )
    {
        FLOG_ERROR("F3DDDSCodec::LoadImageFromVFile, Volume texture, cube texture are nonsupport now!");
		return NULL;
    }

    if( header.surfaceDesc.ddpfPixelFormat.dwFlags & DDPF_RGB )
        image = LoadRGBImage( pFile,header.surfaceDesc );
    else if( header.surfaceDesc.ddpfPixelFormat.dwFlags & DDPF_FOURCC )
    {
        switch( header.surfaceDesc.ddpfPixelFormat.dwFourCC )
        {
        case FOURCC_DXT1:
            image = LoadDXTImage( 1,pFile,header.surfaceDesc );
            break;
        case FOURCC_DXT2:
            image = LoadDXTImage( 2,pFile,header.surfaceDesc );
            break;
        case FOURCC_DXT3:
            image = LoadDXTImage( 3,pFile,header.surfaceDesc );
            break;
        case FOURCC_DXT4:
            image = LoadDXTImage( 4,pFile,header.surfaceDesc );
            break;
        case FOURCC_DXT5:
            image = LoadDXTImage( 5,pFile,header.surfaceDesc );
            break;
        }
    }

    return image;
}

/** 保存指定图像的Mipmap
@Param 要保存图像文件的路径
@Param 指向图像对象的指针
@Param 指定的Mipmap层级
*/
void F3DDDSCodec::SaveMipmap( const char* filename,F3DImage* image,size_t level )
{
    fbyte* destBuf;
    EPixelFormat destFormat;
    size_t destSize,pxSize;

    // 获取源图像的一些信息
    size_t width = image->GetWidth( level );
    size_t height = image->GetHeight( level );
    EPixelFormat srcFormat = image->GetPixelFormat();

    if( F3D_HaveAlpha(srcFormat) )
        destFormat = PFT_A8R8G8B8;
    else
        destFormat = PFT_R8G8B8;

    // 将数据转换为指定像素格式
    pxSize = F3D_PixelSize( destFormat );
    destSize = width*height*pxSize;
    destBuf = new fbyte[destSize];
    F3D_ConvertPixelFormat( image->GetImageData(level),srcFormat,destBuf,destFormat,width,height );

    FVFile file;
	if( file.Open(filename,FVFile::VFILE_CREATENEW|FVFile::VFILE_BINARY) )
    {
        delete[] destBuf;
		FLOG_WARNINGF( "F3DDDSCodec::SaveMipmap, Create the image file(%s) failed!",filename );
        return;
    }

    // 开始写入DDS文件
    SDDSHeader header;
    header.dwMagic = F_MAKEFOURCC( 'D','D','S',' ' );

    memset( &header.surfaceDesc,0,sizeof(SDDSSurfaceDesc2) );
    header.surfaceDesc.dwSize = sizeof(SDDSSurfaceDesc2);
    header.surfaceDesc.dwFlags |= DDSD_CAPS;
    header.surfaceDesc.dwFlags |= DDSD_PIXELFORMAT;
    header.surfaceDesc.dwFlags |= DDSD_WIDTH;
    header.surfaceDesc.dwFlags |= DDSD_HEIGHT;
    header.surfaceDesc.dwWidth = (ulong)image->GetWidth();
    header.surfaceDesc.dwHeight = (ulong)image->GetHeight();

    header.surfaceDesc.ddpfPixelFormat.dwSize = sizeof(SDDSPixelFormat);
    header.surfaceDesc.ddpfPixelFormat.dwFlags |= DDPF_RGB;
    header.surfaceDesc.ddpfPixelFormat.dwRGBBitCount = (ulong)pxSize * 8;
    header.surfaceDesc.ddpfPixelFormat.dwRBitMask = 0x00ff0000;
    header.surfaceDesc.ddpfPixelFormat.dwGBitMask = 0x0000ff00;
    header.surfaceDesc.ddpfPixelFormat.dwBBitMask = 0x000000ff;

    if( destFormat == PFT_A8R8G8B8 )
    {
        header.surfaceDesc.ddpfPixelFormat.dwFlags |= DDPF_ALPHAPIXELS;
        header.surfaceDesc.ddpfPixelFormat.dwRGBAlphaBitMask = 0xff000000;
    }

    header.surfaceDesc.ddsCaps.dwCaps1 = DDSCAPS_TEXTURE;

    // 写入文件头
    file.Write( &header,sizeof(SDDSHeader) );

    // 写入像素数据
    file.Write( destBuf,destSize );

    file.Close();
    delete[] destBuf;
}


///////////////////////////////////////////////////////////////////////////

struct DXT_INFO_1
{
    typedef DXT1Block Block;

    enum
    {
        isDXT1 = 1,
        bytesPerBlock = 8,
    };
};

struct DXT_INFO_3
{
    typedef DXT3Block Block;

    enum
    {
        isDXT1 = 1,
        bytesPerBlock = 16,
    };
};

struct DXT_INFO_5
{
    typedef DXT5Block Block;

    enum
    {
        isDXT1 = 1,
        bytesPerBlock = 16,
    };
};

template <typename INFO>
class DXT_BLOCKDECODER_BASE
{
protected:
    Color8888 m_colors[4];
    const typename INFO::Block* m_pBlock;
    size_t m_colorRow;

public:
    void Setup( const fbyte* pBlock )
    {
        m_pBlock = (const typename INFO::Block*)pBlock;
        GetBlockColors( m_pBlock->color,m_colors,INFO::isDXT1 );
    }

    void SetY( size_t y )
    {
        m_colorRow = m_pBlock->color.row[y];
    }

    void GetColor( size_t x,size_t y,Color8888& color )
    {
        size_t bits = (m_colorRow >> (x*2)) & 3;
        color = m_colors[bits];
    }
};

// DXT1的解码器
class DXT_BLOCKDECODER_1 : public DXT_BLOCKDECODER_BASE<DXT_INFO_1>
{
public: typedef DXT_INFO_1 INFO;
};

// DXT3的解码器
class DXT_BLOCKDECODER_3 : public DXT_BLOCKDECODER_BASE<DXT_INFO_3>
{
public:
    typedef DXT_INFO_3 INFO;
    typedef DXT_BLOCKDECODER_BASE<DXT_INFO_3> base;

protected:
    size_t m_alphaRow;

public:
    void SetY( size_t y )
    {
        base::SetY( y );
        m_alphaRow = m_pBlock->alpha.row[y];
    }

    void GetColor( size_t x,size_t y,Color8888& color )
    {
        base::GetColor( x,y,color );

        size_t bits = (m_alphaRow >> (x * 4)) & 0xf;
        color.a = (fbyte)((bits * 0xff) / 0xf);
    }
};

// DXT5的解码器
class DXT_BLOCKDECODER_5 : public DXT_BLOCKDECODER_BASE<DXT_INFO_5>
{
public:
    typedef DXT_INFO_5 INFO;
    typedef DXT_BLOCKDECODER_BASE<DXT_INFO_5> base;

protected:
    size_t m_alphas[8];
    size_t m_alphaBits;
    size_t m_offset;

public:
    void Setup( const fbyte* pBlock )
    {
        base::Setup( pBlock );

        const DXTAlphaBlock3BitLinear& block = m_pBlock->alpha;
        m_alphas[0] = block.alpha[0];
        m_alphas[1] = block.alpha[1];

        if( m_alphas[0] > m_alphas[1] )
        {
            // 8 alpha block
            for( size_t i=0;i<6;i++ )
            {
                m_alphas[i+2] = ((6-i)*m_alphas[0] + (1+i)*m_alphas[1] + 3) / 7;
            }
        }
        else
        {
            // 6 alpha block
            for( size_t i=0;i<4;i++ )
            {
                m_alphas[i+2] = ((4-i)*m_alphas[0] + (1+i)*m_alphas[1] + 2) / 5;
            }

            m_alphas[6] = 0;
            m_alphas[7] = 0xff;
        }
    }

    void SetY( size_t y )
    {
        base::SetY( y );

        size_t i = y / 2;
        const DXTAlphaBlock3BitLinear& block = m_pBlock->alpha;
        m_alphaBits = (size_t)block.data[i*3+0] | ((size_t)block.data[i*3+1] << 8) |
            ((size_t)block.data[i*3+2] << 16);
        m_offset = (y & 1) * 12;
    }

    void GetColor( size_t x,size_t y,Color8888& color )
    {
        base::GetColor( x,y,color );

        size_t bits = (m_alphaBits >> (x * 3 + m_offset)) & 7;
        color.a = (fbyte)m_alphas[bits];
    }
};


// 获取Block的颜色值
void GetBlockColors( const DXTColBlock& block,Color8888 colors[4],bool isDXT1 )
{
    int i;

    // expand F3D_om 565 to 888
    for( i=0;i<2;i++ )
    {
        colors[i].a = 0xff;

        /*
        colors[i].r = (BYTE)(block.colors[i].r * 0xff / 0x1f);
        colors[i].g = (BYTE)(block.colors[i].g * 0xff / 0x3f);
        colors[i].b = (BYTE)(block.colors[i].b * 0xff / 0x1f);
        */
        colors[i].r = (fbyte)((block.colors[i].r << 3U) | (block.colors[i].r >> 2U));
        colors[i].g = (fbyte)((block.colors[i].g << 2U) | (block.colors[i].g >> 4U));
        colors[i].b = (fbyte)((block.colors[i].b << 3U) | (block.colors[i].b >> 2U));
    }

    ushort* wCol = (ushort*)block.colors;
    if( wCol[0] > wCol[1] || !isDXT1 )
    {
        // 4 color block
        for( i=0;i<2;i++ )
        {
            colors[i+2].a = 0xff;
            colors[i+2].r = (fbyte)((ushort(colors[0].r) * (2 - i) + ushort(colors[1].r) * (1 + i)) / 3);
            colors[i+2].g = (fbyte)((ushort(colors[0].g) * (2 - i) + ushort(colors[1].g) * (1 + i)) / 3);
            colors[i+2].b = (fbyte)((ushort(colors[0].b) * (2 - i) + ushort(colors[1].b) * (1 + i)) / 3);
        }
    }
    else
    {
        // 3 color block, number 4 is transparent
        colors[2].a = 0xff;
        colors[2].r = (fbyte)((ushort(colors[0].r) + ushort(colors[1].r)) / 2);
        colors[2].g = (fbyte)((ushort(colors[0].g) + ushort(colors[1].g)) / 2);
        colors[2].b = (fbyte)((ushort(colors[0].b) + ushort(colors[1].b)) / 2);

        colors[3].a = 0x00;
        colors[3].g = 0x00;
        colors[3].b = 0x00;
        colors[3].r = 0x00;
    }
}

// 对指定的DXT块进行解码
template <typename DECODER>
void DecodeDXTBlock( fbyte* destData,const fbyte* srcBlock,size_t width,
                     size_t height,size_t pitch )
{
    DECODER decoder;

    decoder.Setup( srcBlock );
    for( size_t y=0;y<height;y++ )
    {
        fbyte* dest = destData + y * pitch;

        decoder.SetY( y );
        for( size_t x=0;x<width;x++ )
        {
            decoder.GetColor( x,y,(Color8888&)*dest );
            dest += 4;
        }
    }
}

// 加载普通格式的DDS文件
F3DImage* LoadRGBImage( FVFile* file,const SDDSSurfaceDesc2& desc )
{
    F3DImage* image;

    bool useAlpha = false;
    if( (desc.ddpfPixelFormat.dwFlags & DDPF_ALPHAPIXELS) ||
        (desc.ddpfPixelFormat.dwFlags & DDPF_ALPHA) )
    {
        useAlpha = true;
    }

    // 目标像素的大小
    size_t pxSize = desc.ddpfPixelFormat.dwRGBBitCount / 8;

    // 获取目标像素格式
    EPixelFormat destFormat = F3D_PixelFormat( desc.ddpfPixelFormat.dwRGBBitCount,
        desc.ddpfPixelFormat.dwRBitMask,desc.ddpfPixelFormat.dwGBitMask,desc.ddpfPixelFormat.dwBBitMask,
        desc.ddpfPixelFormat.dwRGBAlphaBitMask,useAlpha );

    size_t linePitch;
    if( desc.ddpfPixelFormat.dwFlags & DDSD_PITCH )
        linePitch = desc.dwPitchOrLinearSize;
    else
        linePitch = desc.dwWidth * pxSize;

    size_t lineSize = desc.dwWidth * pxSize;
    size_t lineRest = lineSize - lineSize;

    // 创建一个图像对象
    image = new F3DImage( desc.dwWidth,desc.dwHeight,destFormat );

    // 读取像素数据
    for( size_t y=0;y<desc.dwHeight;y++ )
    {
        fbyte* dest = image->GetImageData() + lineSize;

        file->Read( dest,lineSize );
        file->Seek( (long)lineRest,FVFile::CURRENT );
    }

    return image;
}

// 加载DXT压缩文理
F3DImage* LoadDXTImage( int DXT,FVFile* file,const SDDSSurfaceDesc2& desc )
{
    F3DImage* image;

    size_t width = desc.dwWidth;
    size_t height = desc.dwHeight;

    bool useAlpha = false;
    if( (desc.ddpfPixelFormat.dwFlags & DDPF_ALPHAPIXELS) ||
        (desc.ddpfPixelFormat.dwFlags & DDPF_ALPHA) )
    {
        useAlpha = true;
    }

    size_t pxSize = 32 / 8;

    // 获取目标像素格式
    EPixelFormat destFormat = F3D_PixelFormat( desc.ddpfPixelFormat.dwRGBBitCount,
        desc.ddpfPixelFormat.dwRBitMask,desc.ddpfPixelFormat.dwGBitMask,desc.ddpfPixelFormat.dwBBitMask,
        desc.ddpfPixelFormat.dwRGBAlphaBitMask,useAlpha );

    // 创建图像对象
    image = new F3DImage( desc.dwWidth,desc.dwHeight,destFormat );

    // 计算每行像素的字节数
    size_t linePitch = ((width * 32) + 7) / 8;

    switch( DXT )
    {
    case 1: LoadDXTHelper<DXT_BLOCKDECODER_1>( file,image,width,height,linePitch ); break;
    case 2: LoadDXTHelper<DXT_BLOCKDECODER_3>( file,image,width,height,linePitch ); break;
    case 3: LoadDXTHelper<DXT_BLOCKDECODER_3>( file,image,width,height,linePitch ); break;
    case 4: LoadDXTHelper<DXT_BLOCKDECODER_5>( file,image,width,height,linePitch ); break;
    case 5: LoadDXTHelper<DXT_BLOCKDECODER_5>( file,image,width,height,linePitch ); break;
    }

    return image;
}

// 加载DXT压缩纹理的辅助函数
template <typename DECODER>
void LoadDXTHelper( FVFile* file,F3DImage* image,size_t width,
                    size_t height,size_t pitch )
{
    typedef typename DECODER::INFO INFO;
    typedef typename INFO::Block Block;

    size_t widthRest = width & 3;
    size_t heightRest = height & 3;
    size_t inputLine = (width + 3) / 4;

    size_t y = 0;
    Block* input_buffer = new Block[(width+3)/4];

    if( height >= 4 )
    {
        for( ;y<height;y+=4 )
        {
            file->Read( input_buffer,sizeof(Block)*inputLine );

            fbyte* src = (fbyte*)input_buffer;
            fbyte* dest = image->GetImageData() + pitch * y;

            if( width >= 4 )
            {
                for( size_t x=0;x<width;x+=4 )
                {
                    DecodeDXTBlock<DECODER>( dest,src,4,4,pitch );
                    src += INFO::bytesPerBlock;
                    dest += 4 * 4;
                }
            }

            if( widthRest )
            {
                DecodeDXTBlock<DECODER>( dest,src,widthRest,4,pitch );
            }
        }
    }

    if( heightRest )
    {
        file->Read( input_buffer,sizeof(Block)*inputLine );

        fbyte* src = (fbyte*)input_buffer;
        fbyte* dest = image->GetImageData() + pitch * y;

        if( width >= 4 )
        {
            for( size_t x=0;x<width;x+=4 )
            {
                DecodeDXTBlock<DECODER>( dest,src,4,heightRest,pitch );
                src += INFO::bytesPerBlock;
                dest += 4 * 4;
            }
        }

        if( widthRest )
        {
            DecodeDXTBlock<DECODER>( dest,src,widthRest,heightRest,pitch );
        }
    }

    delete[] input_buffer;
}