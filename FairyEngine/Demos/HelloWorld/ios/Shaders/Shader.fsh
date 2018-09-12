//
//  Shader.fsh
//  HelloWorld
//
//  Created by Yish on 16/1/24.
//  Copyright © 2016年 YishSoft. All rights reserved.
//

varying lowp vec4 colorVarying;

void main()
{
    gl_FragColor = colorVarying;
}
