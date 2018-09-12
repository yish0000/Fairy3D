/*
 * ------------------------------------------------------------------------
 *  Name:   FairyExplorer.h
 *  Desc:   本文件定义了一个资源包中文件的树状结构。
 *  Author: Yish
 *  Date:   2011/9/7
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */

#ifndef __FAIRY_EXPLORER_H__
#define __FAIRY_EXPLORER_H__

//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include <FVFileSystem.h>
#include <FFpkFilePack.h>
#include <FSysAPI.h>

///////////////////////////////////////////////////////////////////////////

// 文件包目录分隔符
#define FPK_DIR_CHAR '/'
#define FPK_DIR_STR "/"

// 系统目录分隔符
#if FAIRY_PLATFORM == FAIRY_PLATFORM_WINDOWS
    #define SYS_DIR_CHAR '\\'
    #define SYS_DIR_STR "\\"
#else
    // Linux or else ?
    #define SYS_DIR_CHAR '/'
    #define SYS_DIR_STR "/"
#endif

struct SConfigData
{
	int compress;           // 压缩算法
	int bufferLevel;        // 缓冲区级别
	AStringList extList;    // 不压缩文件列表
};

extern SConfigData g_Config;

// 文件包的信息
struct SPackageInfo
{
    uint32 fileCount;		// 文件个数
    uint32 folderCount;		// 目录个数
    uint64 origSize;		// 文件总大小
    uint32 compressSize;	// 压缩后大小
    float compressDegree;	// 压缩率
};

struct IFileDupHandler
{
	// 返回true, 覆盖，返回false，放弃
	virtual bool Handle(const AString& filename) = 0;
};

/** 该类为压缩包文件构建了一棵树
*/
class CFairyExplorer
{
public:
    
    friend bool AddPackFile( const AString& path, void* param );

    struct PKNode
    {
        AString strName;                // 该文件的名字
        bool bIsDirectory;              // 是否目录
        uint32 nFileSize;               // 文件的原始大小
        uint32 nCompressedSize;         // 压缩后的大小
        PKNode* pParent;                // 父节点
        std::list<PKNode> Children;     // 子节点列表
        void* pUserData;                // 用于存放用户数据

        PKNode() : pUserData(NULL) {}
    };

    enum
    {
        FAIRY_FIND_DIR = 1,
        FAIRY_FIND_FILE = 2,
        FAIRY_FIND_ANY = 3,
    };

    // 回调函数
    typedef bool (*FPKCallback) ( const AString& filename );

private:
    FVFilePack* m_pPackage;         // 压缩包的指针
    FPKCallback m_pCallback;        // 处理压缩包时的回调函数
    PKNode m_Root;                  // 文档的根目录
    PKNode* m_pCurNode;             // 当前的结点
    AString m_sCurPath;             // 当前的目录名字
	IFileDupHandler* m_pDupHandler; // 文件重复处理

    AString m_sWorkingPath;         //
    AString m_sBasePath;            //
    
public:
    CFairyExplorer(void);
    virtual ~CFairyExplorer(void);

    // 操作文件包
    bool Open( const AString& filename );
    bool Create( const AString& filename );
    void SaveAs( const AString& filename );
    void Close(void);
    bool IsOpen(void) const { return (m_pPackage != NULL); }

    // 设置处理压缩包的回调函数
    void SetCallback( FPKCallback cbFunc ) { m_pCallback = cbFunc; }
	// 设置文件重复的处理
	void SetFileDupHandler(IFileDupHandler* pHandler) { m_pDupHandler = pHandler; }

    // 进入指定的目录
    bool EnterFolder( const AString& path );

    // 添加指定的文件到压缩包
    bool Add( const AString& from, const AString& to );

    // 解压指定的节点
    bool Extract( const AString& from, const AString& to );

    // 移除指定的节点
    bool Remove( const AString& path );

    // 整理文件包碎片
    bool Defrag(void);

	// 在指定的目录下搜索文件
	bool SearchFiles(const AString& folder, const AString& strFile, AStringList& fileList);
	// 在当前目录下搜索文件
	bool SearchFiles(const AString& strFile, AStringList& fileList);
	// 在指定的目录下搜索文件
	bool SearchFiles(const AString& folder, const AString& strFile, std::vector<const PKNode*>& fileList);
	// 在当前目录下搜索文件
	bool SearchFiles(const AString& strFile, std::vector<const PKNode*>& fileList);

    // 获取当前所在目录的路径（包内）
    const AString& GetCurrentPath(void) const { return m_sCurPath; }

    // 获取指定节点的文件个数
    uint32 GetNodeFileCount( const PKNode* node );
    // 获取指定目录的文件个数（系统）
    uint32 GetDiskFileCount( const AString& dir );

    // 获取文件包的信息
    void GetPackageInfo( SPackageInfo& info );

    // 节点相关的操作
    void SetCurrentNode( const PKNode* pNode );
    const PKNode* GetCurrentNode(void) const { return m_pCurNode; }
    const PKNode* GetRootNode(void) const { return &m_Root; }
	const PKNode* GetNodeByPath(const AString& path);

    // 获取当前文件包指针
    FVFilePack* GetPackage(void) { return m_pPackage; }
	bool IsReadonly(void) const { return m_pPackage ? m_pPackage->IsReadonly() : false; }

    // 获取当前压缩包的名字
	const AString& GetPackageName(void) const;

private:
    void Build(void);

    // 添加指定的文件
    bool AddFile( const AString& sFile, const AString& to );

    // 添加指定文件到节点树
    void InsertFileToTree( const AString& filename, uint32 nSize, uint32 nCompressedSize, bool bCheckExisted=false );

    // 解压指定的文件
    bool ExtractFile( const AString& filename, const AString& output );
    bool ExtractFileR( PKNode* pNode, const AString& path, const AString& output );

    // 移除指定的节点
    void RemoveNodeR( PKNode* pNode, AString& path );

    // 查找指定节点的子节点
    PKNode* FindChildNode( int iFind, PKNode* node, const AString& name );
    PKNode* FindChildNodeR( int iFind, PKNode* node, const AString& name );

	// 递归搜索文件
	bool SearchFileR(PKNode* node, const AString& strFile, AStringList& fileList);
	bool SearchFileR(PKNode* node, const AString& strFile, std::vector<const PKNode*>& fileList);

    // 获取指定节点的文件个数
    void GetNodeCount( const PKNode* pNode, uint32& fileCount );

    // 获取指定节点的路径
    void GetNodePath( const PKNode* pNode, AString& path ) const;

    // 枚举当前节点的信息
    void EnumNodeInfo( const PKNode* pNode, SPackageInfo& info );
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FAIRY_EXPLORER_H__