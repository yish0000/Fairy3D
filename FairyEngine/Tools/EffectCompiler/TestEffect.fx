FFX                MainSampler	   sampler2D            asdf      8866983   MaskSampler	   sampler2D             DetailSampler1	   sampler2D             g_NumDetails   int            asdf      8866983
   g_UseLight   int             g_MainScale   float             g_DetailScale1   float          �  
        struct PS_OUTPUT
        {
            float4 RGBColor : COLOR0;   // Pixel color.
        };

        // Pixel shader function.
        PS_OUTPUT PixelFunc( float2 vTexCoord0 : TEXCOORD0,
                             float2 vTexCoord1 : TEXCOORD1 )
        {
            PS_OUTPUT Output;
            float4 diffuse;

            float4 mainColor = tex2D( MainSampler,vTexCoord0*g_MainScale );
            float4 maskColor = tex2D( MaskSampler,vTexCoord1 );

            float4 detail1 = tex2D( DetailSampler1,vTexCoord0*g_DetailScale1 );
            float4 detail2 = tex2D( DetailSampler2,vTexCoord0*g_DetailScale2 );
            float4 detail3 = tex2D( DetailSampler3,vTexCoord0*g_DetailScale3 );
            float4 detail4 = tex2D( DetailSampler4,vTexCoord0*g_DetailScale4 );

            diffuse = mainColor;

            if( g_NumDetails >= 1 ) diffuse = maskColor[2]*detail1+(1-maskColor[2])*diffuse;
            if( g_NumDetails >= 2 ) diffuse = maskColor[1]*detail2+(1-maskColor[1])*diffuse;
            if( g_NumDetails >= 3 ) diffuse = maskColor[0]*detail3+(1-maskColor[0])*diffuse;
            if( g_NumDetails == 4 ) diffuse = maskColor[3]*detail4+(1-maskColor[3])*diffuse;

            if( g_UseLight )
            {
                float4 light = tex2D( LightSampler,vTexCoord1 );
                diffuse[0] *= light[0];
                diffuse[1] *= light[1];
                diffuse[2] *= light[2];
            }

            Output.RGBColor = diffuse;
            return Output;
        }
           MultiTexture      AnotherName   
   ForTerrain      p0      Second      sb_pass           PixelFunc()   ps_2_0                        ����