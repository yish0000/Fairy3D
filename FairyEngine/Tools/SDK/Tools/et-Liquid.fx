FFX    
         
   g_TexScale           x�         	   g_Opacity           x�            g_Distortion           x�            g_Speed           x�         	   g_Surface           x�                                                g_ReflectMap           x�                                             	   g_DudvMap           x�                                                g_CameraPos           x�                        g_WorldViewProj                                                                                 
   g_LightPos           x�                     C  
        struct VS_OUTPUT
        {
            float4 Pos       : POSITION;
            float2 DupCoord0 : TEXCOORD0;
            float4 ViewCoord : TEXCOORD1;
        };
        VS_OUTPUT LiquidVS( float3 vPosition  : POSITION,
                            float2 vTexCoord0 : TEXCOORD0 )
        {
            VS_OUTPUT Out;

            float4x4 mat =
            {
                0.5f,0.0f,0.0f,0.0f,
                0.0f,0.5f,0.0f,0.0f,
                0.0f,0.0f,0.5f,0.0f,
                0.5f,0.5f,0.5f,1.0f
            };

            Out.Pos = mul(float4(vPosition,1.0f),g_WorldViewProj);
            Out.ViewCoord = mul(float4(vPosition,1.0f),g_WorldViewProj*mat);
            Out.DupCoord0 = vTexCoord0 * 0.2f;
            return Out;
        }

        float4 LiquidPS( float4 vPos      : POSITION,
                         float2 DupCoord0 : TEXCOORD0,
                         float4 ViewCoord : TEXCOORD1 ) : COLOR
        {
            float4 finalColor;

            // get normal from dudv map
            float3 norm = normalize(tex2D(g_DudvMap,DupCoord0).xyz*2.0f-1.0f);
            float3 dudv = norm * 0.5f;

            // compute reflection texture coordinates and distortion
            float2 txp = ViewCoord.xy / ViewCoord.w;
            float4 refl = tex2D(g_ReflectMap,txp+dudv.xy);

            // compute lighting and specular
            //float3 lightDir = g_LightPos.xyz;

            //float NDotL = max(dot(norm,lightDir),0.0f);
            //NDotL = (NDotL * 0.5f) + 0.5f;

            finalColor = refl;

            float3 view = g_CameraPos - vPos;
            float dist = length(view);
            view = normalize(view);

            float alpha = 1.0f-clamp(view.y,0.0f,1.0f)+dist/5.0f;
            alpha = clamp(alpha,0.4f,1.0f);
            finalColor.a = alpha;
            return finalColor;
        }
           LiquidTechnique          p0       LiquidVS   auto   LiquidPS   ps_2_0                  