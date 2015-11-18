xof 0302txt 0064
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

Header{
1;
0;
1;
}

Mesh {
 136;
 0.00000;100.00001;0.00000;,
 -1.41382;100.00001;1.41421;,
 0.00000;100.00001;2.00000;,
 -1.41382;100.00001;-1.41421;,
 -2.00000;100.00001;0.00000;,
 0.00000;100.00001;-2.00000;,
 1.41421;100.00001;-1.41421;,
 2.00000;100.00001;0.00000;,
 1.41421;100.00001;1.41382;,
 -1.41421;92.58333;1.41421;,
 -2.00000;92.58333;0.00000;,
 -3.84521;54.14085;0.22539;,
 -2.71898;54.14085;2.94437;,
 -1.41421;92.58333;-1.41421;,
 -2.71898;54.14085;-2.49359;,
 0.00000;92.58333;-2.00000;,
 0.00000;54.14085;-3.61982;,
 1.41421;92.58333;-1.41421;,
 2.71898;54.14085;-2.49359;,
 2.00000;92.58333;0.00000;,
 3.84521;54.14085;0.22539;,
 1.41421;92.58333;1.41421;,
 2.71898;54.14085;2.94437;,
 0.00000;92.58333;2.00000;,
 0.00000;54.14085;4.07060;,
 -1.41421;92.58333;1.41421;,
 -2.71898;54.14085;2.94437;,
 -7.71408;6.85465;-3.97841;,
 -4.99601;9.29192;-4.78444;,
 -6.39055;6.85465;-7.17369;,
 0.00000;9.24942;7.99921;,
 -7.06542;9.19631;2.64284;,
 -9.03761;6.85465;-0.14171;,
 0.00000;6.85465;9.40447;,
 7.06542;9.19631;2.64284;,
 9.03761;6.85465;-0.14171;,
 7.71408;6.85465;-3.97841;,
 6.39055;6.85465;-7.17369;,
 4.99601;9.29192;-4.78444;,
 3.19528;6.85465;-8.49721;,
 0.00000;6.85465;-9.17763;,
 0.00000;9.34504;-6.83918;,
 -3.19528;6.85465;-8.49722;,
 -8.74408;37.34447;-7.73997;,
 -8.22033;35.98415;-1.04367;,
 -5.03805;47.44413;-1.11150;,
 -4.61144;48.47919;-4.41725;,
 0.00000;36.07269;2.97993;,
 0.00000;46.97385;2.12852;,
 8.22033;35.98415;-1.04367;,
 5.03805;47.44412;-1.11150;,
 8.74408;37.34447;-7.73997;,
 4.61144;48.47919;-4.41725;,
 0.00000;39.12067;-12.83885;,
 0.00000;49.61446;-7.46233;,
 -12.31943;24.71363;-7.28155;,
 -10.52753;24.77264;2.87052;,
 0.00000;25.93159;11.63714;,
 10.52753;24.77264;2.87052;,
 12.31942;24.71363;-7.28155;,
 0.00000;25.08199;-14.22765;,
 0.00000;46.97385;2.12852;,
 0.00000;54.14085;4.07060;,
 -2.71898;54.14085;2.94437;,
 -5.03805;47.44413;-1.11150;,
 -3.84521;54.14085;0.22539;,
 -2.71898;54.14085;-2.49359;,
 -4.61144;48.47919;-4.41725;,
 -2.71898;54.14085;-2.49359;,
 0.00000;54.14085;-3.61982;,
 0.00000;49.61446;-7.46233;,
 2.71898;54.14085;-2.49359;,
 4.61144;48.47919;-4.41725;,
 2.71898;54.14085;-2.49359;,
 3.84521;54.14085;0.22539;,
 5.03805;47.44412;-1.11150;,
 2.71898;54.14085;2.94437;,
 -8.00648;0.00000;-0.78313;,
 0.00000;0.00000;-0.78313;,
 0.00000;0.00000;9.26779;,
 -10.86569;0.00000;-6.47956;,
 -5.66144;0.00000;-6.44457;,
 -4.48463;0.00000;-12.76684;,
 0.00000;0.00000;-8.78961;,
 4.48463;0.00000;-12.76684;,
 5.66144;0.00000;-6.44457;,
 10.86569;0.00000;-6.47956;,
 8.00648;0.00000;-0.78313;,
 -8.00648;0.00000;-0.78313;,
 0.00000;0.00000;9.26779;,
 -5.66144;0.00000;-6.44457;,
 0.00000;0.00000;-8.78961;,
 5.66144;0.00000;-6.44457;,
 8.00648;0.00000;-0.78313;,
 -4.48463;0.00000;-12.76684;,
 -10.86569;0.00000;-6.47956;,
 10.86569;0.00000;-6.47956;,
 4.48463;0.00000;-12.76684;,
 0.00000;13.31141;-10.94028;,
 10.13220;13.04830;-5.18409;,
 7.65954;12.83193;4.55394;,
 0.00000;13.00317;10.62573;,
 -7.65954;12.83193;4.55394;,
 -10.13220;13.04830;-5.18409;,
 -1.41421;92.58333;1.41421;,
 0.00000;92.58333;2.00000;,
 0.00000;93.06927;1.23646;,
 -0.87431;93.06927;0.87431;,
 -1.41421;92.58333;-1.41421;,
 -2.00000;92.58333;0.00000;,
 -1.23647;93.06927;0.00000;,
 -0.87431;93.06927;-0.87431;,
 0.00000;92.58333;-2.00000;,
 0.00000;93.06927;-1.23646;,
 1.41421;92.58333;-1.41421;,
 0.87431;93.06927;-0.87431;,
 2.00000;92.58333;0.00000;,
 1.23646;93.06927;0.00000;,
 1.41421;92.58333;1.41421;,
 0.87431;93.06927;0.87431;,
 0.00000;95.28197;1.23646;,
 -0.87431;95.28197;0.87431;,
 -1.23647;95.28197;0.00000;,
 -0.87431;95.28197;-0.87431;,
 0.00000;95.28197;-1.23646;,
 0.87431;95.28197;-0.87431;,
 1.23646;95.28197;0.00000;,
 0.87431;95.28197;0.87431;,
 0.00000;95.76795;2.00000;,
 -1.41382;95.76795;1.41421;,
 -2.00000;95.76795;0.00000;,
 -1.41382;95.76795;-1.41421;,
 0.00000;95.76795;-2.00000;,
 1.41421;95.76795;-1.41421;,
 2.00000;95.76795;0.00000;,
 1.41421;95.76795;1.41382;;
 
 118;
 3;0,1,2;,
 3;3,4,0;,
 3;5,3,0;,
 3;6,5,0;,
 3;7,6,0;,
 3;8,0,2;,
 4;9,10,11,12;,
 4;10,13,14,11;,
 4;13,15,16,14;,
 4;16,15,17,18;,
 4;17,19,20,18;,
 4;19,21,22,20;,
 4;21,23,24,22;,
 4;23,25,26,24;,
 3;27,28,29;,
 4;30,31,32,33;,
 4;34,30,33,35;,
 3;36,37,38;,
 4;39,40,41,38;,
 4;41,40,42,28;,
 4;43,44,45,46;,
 4;45,44,47,48;,
 4;48,47,49,50;,
 4;49,51,52,50;,
 4;51,53,54,52;,
 4;53,43,46,54;,
 4;55,56,44,43;,
 4;47,44,56,57;,
 4;49,47,57,58;,
 4;58,59,51,49;,
 4;59,60,53,51;,
 4;60,55,43,53;,
 4;61,62,63,64;,
 4;64,65,66,67;,
 4;68,69,70,67;,
 4;69,71,72,70;,
 4;73,74,75,72;,
 3;76,75,74;,
 3;77,78,79;,
 3;77,80,81;,
 3;81,82,83;,
 3;83,84,85;,
 3;85,86,87;,
 3;78,87,79;,
 4;33,32,88,89;,
 3;90,27,29;,
 3;91,42,40;,
 3;92,39,37;,
 3;93,36,35;,
 4;89,93,35,33;,
 3;42,90,29;,
 3;91,94,42;,
 3;27,88,32;,
 3;95,88,27;,
 3;36,92,37;,
 3;96,92,36;,
 3;39,91,40;,
 3;97,91,39;,
 3;90,95,27;,
 3;94,90,42;,
 3;92,97,39;,
 3;93,96,36;,
 4;98,60,59,99;,
 4;99,59,58,100;,
 4;58,57,101,100;,
 4;57,56,102,101;,
 4;56,55,103,102;,
 4;55,60,98,103;,
 4;41,98,99,38;,
 4;38,99,100,34;,
 4;100,101,30,34;,
 4;101,102,31,30;,
 4;102,103,28,31;,
 4;103,98,41,28;,
 3;85,87,78;,
 3;83,85,78;,
 3;81,83,78;,
 3;77,81,78;,
 3;28,42,29;,
 3;39,38,37;,
 4;31,28,27,32;,
 4;36,38,34,35;,
 3;64,63,65;,
 4;76,62,61,75;,
 3;8,7,0;,
 3;0,4,1;,
 4;104,105,106,107;,
 4;108,109,110,111;,
 4;112,108,111,113;,
 4;114,112,113,115;,
 4;116,114,115,117;,
 4;105,118,119,106;,
 4;118,116,117,119;,
 4;109,104,107,110;,
 4;107,106,120,121;,
 4;111,110,122,123;,
 4;113,111,123,124;,
 4;115,113,124,125;,
 4;117,115,125,126;,
 4;106,119,127,120;,
 4;119,117,126,127;,
 4;110,107,121,122;,
 4;121,120,128,129;,
 4;123,122,130,131;,
 4;124,123,131,132;,
 4;125,124,132,133;,
 4;126,125,133,134;,
 4;120,127,135,128;,
 4;127,126,134,135;,
 4;122,121,129,130;,
 4;129,128,2,1;,
 4;131,130,4,3;,
 4;132,131,3,5;,
 4;133,132,5,6;,
 4;134,133,6,7;,
 4;128,135,8,2;,
 4;135,134,7,8;,
 4;130,129,1,4;;
 
 MeshMaterialList {
  1;
  118;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0;;
  Material {
   0.320000;0.320000;0.320000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  131;
  -0.857608;0.514304;-0.000023;,
  -0.605708;0.516007;0.605684;,
  -0.607132;0.512596;-0.607153;,
  0.000000;0.511888;-0.859052;,
  0.607132;0.512596;-0.607153;,
  0.857608;0.514304;-0.000022;,
  0.605709;0.516007;0.605684;,
  0.000000;0.516712;0.856159;,
  -0.726575;-0.023170;0.686697;,
  -0.991209;0.123091;0.048519;,
  -0.746433;0.232876;-0.623383;,
  0.000000;0.295168;-0.955446;,
  0.746433;0.232876;-0.623383;,
  0.991209;0.123090;0.048520;,
  0.726575;-0.023170;0.686697;,
  0.000000;-0.123218;0.992379;,
  -0.670488;0.342923;-0.657913;,
  -0.714218;0.012476;0.699812;,
  -0.844468;0.137736;0.517593;,
  -0.982321;0.046633;0.181301;,
  -0.584440;0.210192;0.783741;,
  0.714218;0.012476;0.699812;,
  0.844468;0.137736;0.517593;,
  0.716067;0.494219;-0.492945;,
  0.982321;0.046633;0.181301;,
  0.000000;0.370810;-0.928709;,
  0.191046;-0.145302;-0.970767;,
  -0.860094;0.310243;-0.404954;,
  -0.804379;0.314234;0.504213;,
  -0.880300;0.119160;0.459209;,
  -0.796927;0.404076;-0.449032;,
  0.000000;0.326942;0.945044;,
  0.000000;-0.100402;0.994947;,
  0.804379;0.314234;0.504213;,
  0.880300;0.119160;0.459209;,
  0.860094;0.310243;-0.404954;,
  0.796927;0.404076;-0.449032;,
  0.531928;0.248726;-0.809437;,
  0.502986;0.437249;-0.745533;,
  0.000000;-1.000000;-0.000000;,
  -0.099856;0.376995;-0.920817;,
  -0.932935;0.315786;-0.172949;,
  0.932935;0.315786;-0.172949;,
  0.099856;0.376995;-0.920817;,
  -0.000000;-1.000000;0.000000;,
  0.000000;-0.500047;-0.865998;,
  0.609092;-0.554074;-0.567459;,
  0.808626;-0.351702;0.471624;,
  0.596102;-0.268795;0.756579;,
  -0.808626;-0.351702;0.471624;,
  -0.913583;-0.387789;0.122419;,
  0.000000;0.728614;-0.684924;,
  0.475761;0.746857;-0.464604;,
  0.630381;-0.186216;0.753620;,
  -0.630381;-0.186216;0.753620;,
  -0.756475;-0.351253;0.551695;,
  -0.475761;0.746857;-0.464604;,
  -0.379662;0.843631;0.379662;,
  0.000000;0.843631;0.536923;,
  -0.379662;0.843631;-0.379662;,
  -0.536924;0.843631;0.000000;,
  0.000000;0.843631;-0.536923;,
  0.379662;0.843631;-0.379662;,
  0.536923;0.843631;0.000000;,
  0.379662;0.843631;0.379662;,
  -0.614673;-0.494308;0.614684;,
  0.000014;-0.494318;0.869281;,
  -0.614673;-0.494308;-0.614684;,
  -0.869285;-0.494311;0.000000;,
  -0.000013;-0.494337;-0.869271;,
  0.614662;-0.494349;-0.614662;,
  0.869271;-0.494337;0.000013;,
  0.614684;-0.494309;0.614673;,
  -0.379729;-0.843563;0.379747;,
  0.000025;-0.843580;0.537004;,
  -0.379729;-0.843563;-0.379747;,
  -0.537014;-0.843573;0.000000;,
  -0.000023;-0.843599;-0.536974;,
  0.379684;-0.843612;-0.379684;,
  0.536974;-0.843599;0.000023;,
  0.379747;-0.843563;0.379729;,
  0.000000;1.000000;0.000000;,
  -0.531928;0.248726;-0.809437;,
  -0.502986;0.437249;-0.745533;,
  0.584440;0.210192;0.783741;,
  0.495050;-0.072640;-0.865823;,
  -0.502783;0.095982;-0.859067;,
  -0.495050;-0.072640;-0.865823;,
  -0.827615;0.075590;0.556183;,
  -0.751862;-0.092094;0.652856;,
  -0.675300;0.106390;-0.729829;,
  0.000000;0.156906;-0.987613;,
  0.675300;0.106390;-0.729829;,
  0.379041;-0.137654;-0.915085;,
  0.995581;0.071075;0.061367;,
  0.894259;0.337534;-0.293890;,
  0.751862;-0.092094;0.652856;,
  0.627965;0.322929;-0.708080;,
  -0.939492;-0.110998;-0.324091;,
  -0.894259;0.337534;-0.293890;,
  -0.867613;0.240008;0.435481;,
  -0.006322;0.340204;-0.940330;,
  -0.627965;0.322929;-0.708080;,
  0.006322;0.340204;-0.940330;,
  -0.941936;0.286386;-0.175329;,
  0.941936;0.286386;-0.175329;,
  0.867613;0.240008;0.435481;,
  0.963243;-0.173420;0.205153;,
  -0.596102;-0.268795;0.756579;,
  -0.411135;-0.455830;-0.789422;,
  -0.000000;-0.685398;-0.728169;,
  0.633589;-0.682633;-0.364111;,
  0.813433;-0.580727;0.032920;,
  -0.633589;-0.682633;-0.364111;,
  -0.666473;0.717163;-0.203694;,
  0.666473;0.717163;-0.203695;,
  -0.707106;0.000000;0.707107;,
  0.000000;0.000000;1.000000;,
  -0.707106;0.000000;-0.707107;,
  -1.000000;0.000000;0.000000;,
  0.000000;0.000000;-1.000000;,
  0.707107;0.000000;-0.707107;,
  1.000000;0.000000;0.000000;,
  0.707107;0.000000;0.707107;,
  -0.707058;0.000000;0.707156;,
  0.000069;0.000000;1.000000;,
  -0.707058;0.000000;-0.707156;,
  -1.000000;0.000000;0.000000;,
  -0.000049;0.000000;-1.000000;,
  1.000000;0.000000;0.000049;,
  0.707156;0.000000;0.707058;;
  118;
  3;81,81,81;,
  3;81,81,81;,
  3;81,81,81;,
  3;81,81,81;,
  3;81,81,81;,
  3;81,81,81;,
  4;1,0,9,8;,
  4;0,2,10,9;,
  4;2,3,11,10;,
  4;11,3,4,12;,
  4;4,5,13,12;,
  4;5,6,14,13;,
  4;6,7,15,14;,
  4;7,1,8,15;,
  3;41,56,16;,
  4;54,55,17,17;,
  4;53,53,21,21;,
  3;42,23,52;,
  4;43,25,51,52;,
  4;51,25,40,56;,
  4;27,28,29,30;,
  4;29,28,31,32;,
  4;32,31,33,34;,
  4;33,35,36,34;,
  4;35,37,38,36;,
  4;82,27,30,83;,
  4;19,18,28,27;,
  4;31,28,18,20;,
  4;33,31,84,22;,
  4;22,24,35,33;,
  4;85,26,37,35;,
  4;86,87,27,82;,
  4;32,15,8,29;,
  4;29,9,10,30;,
  4;10,11,83,30;,
  4;11,12,36,38;,
  4;12,13,34,36;,
  3;14,34,13;,
  3;39,39,39;,
  3;39,44,39;,
  3;39,39,39;,
  3;39,39,39;,
  3;39,44,39;,
  3;39,39,39;,
  4;17,17,88,89;,
  3;90,41,16;,
  3;91,40,25;,
  3;92,43,93;,
  3;94,42,95;,
  4;96,96,21,21;,
  3;40,90,16;,
  3;91,97,40;,
  3;41,98,99;,
  3;100,88,41;,
  3;42,92,23;,
  3;101,92,101;,
  3;43,91,25;,
  3;102,91,43;,
  3;90,103,103;,
  3;104,90,104;,
  3;92,105,105;,
  3;94,106,42;,
  4;45,26,85,46;,
  4;107,24,22,47;,
  4;22,84,48,47;,
  4;20,18,49,108;,
  4;18,19,50,49;,
  4;87,26,45,109;,
  4;110,45,46,111;,
  4;111,46,47,112;,
  4;47,48,53,53;,
  4;108,49,55,54;,
  4;49,50,113,55;,
  4;109,45,110,113;,
  3;39,39,39;,
  3;39,39,39;,
  3;39,39,39;,
  3;39,39,39;,
  3;56,40,16;,
  3;43,52,23;,
  4;114,56,41,99;,
  4;42,52,115,95;,
  3;29,8,9;,
  4;14,15,32,34;,
  3;81,81,81;,
  3;81,81,81;,
  4;1,7,58,57;,
  4;2,0,60,59;,
  4;3,2,59,61;,
  4;4,3,61,62;,
  4;5,4,62,63;,
  4;7,6,64,58;,
  4;6,5,63,64;,
  4;0,1,57,60;,
  4;116,117,66,65;,
  4;118,119,68,67;,
  4;120,118,67,69;,
  4;121,120,69,70;,
  4;122,121,70,71;,
  4;117,123,72,66;,
  4;123,122,71,72;,
  4;119,116,65,68;,
  4;65,66,74,73;,
  4;67,68,76,75;,
  4;69,67,75,77;,
  4;70,69,77,78;,
  4;71,70,78,79;,
  4;66,72,80,74;,
  4;72,71,79,80;,
  4;68,65,73,76;,
  4;124,125,125,124;,
  4;126,127,127,126;,
  4;128,126,126,128;,
  4;121,128,128,121;,
  4;129,121,121,129;,
  4;125,130,130,125;,
  4;130,129,129,130;,
  4;127,124,124,127;;
 }
 MeshTextureCoords {
  136;
  0.500000;0.162500;,
  0.610485;0.045765;,
  0.500000;0.000000;,
  0.610485;0.266735;,
  0.656250;0.156250;,
  0.500000;0.312500;,
  0.389515;0.266735;,
  0.343750;0.156250;,
  0.389515;0.045765;,
  0.625000;0.311560;,
  0.593750;0.311560;,
  0.593750;0.683814;,
  0.625000;0.683814;,
  0.562500;0.311560;,
  0.562500;0.683814;,
  0.531250;0.311560;,
  0.531250;0.683814;,
  0.500000;0.311560;,
  0.500000;0.683814;,
  0.468750;0.311560;,
  0.468750;0.683814;,
  0.437500;0.311560;,
  0.437500;0.683814;,
  0.406250;0.311560;,
  0.406250;0.683814;,
  0.375000;0.311560;,
  0.375000;0.683814;,
  0.578125;0.604849;,
  0.562500;0.584632;,
  0.562500;0.604849;,
  0.406250;0.584632;,
  0.593750;0.584632;,
  0.593750;0.604849;,
  0.406250;0.604849;,
  0.468750;0.584632;,
  0.468750;0.604849;,
  0.484375;0.604849;,
  0.500000;0.604849;,
  0.500000;0.584632;,
  0.515625;0.604849;,
  0.531250;0.604849;,
  0.531250;0.584632;,
  0.546875;0.604849;,
  0.562500;0.324908;,
  0.593750;0.324908;,
  0.593750;0.311560;,
  0.562500;0.311560;,
  0.406250;0.324908;,
  0.406250;0.311560;,
  0.468750;0.324908;,
  0.468750;0.311560;,
  0.500000;0.324908;,
  0.500000;0.311560;,
  0.531250;0.324908;,
  0.531250;0.311560;,
  0.562500;0.448703;,
  0.593750;0.448703;,
  0.406250;0.448703;,
  0.468750;0.448703;,
  0.500000;0.448703;,
  0.531250;0.448703;,
  0.500000;0.000000;,
  0.500000;0.116071;,
  0.531567;0.129147;,
  0.656250;0.156250;,
  0.544643;0.160714;,
  0.531567;0.192282;,
  0.610485;0.266735;,
  0.531567;0.192281;,
  0.500000;0.205357;,
  0.500000;0.312500;,
  0.468433;0.192282;,
  0.389515;0.266735;,
  0.468433;0.192281;,
  0.455357;0.160714;,
  0.343750;0.156250;,
  0.468433;0.129147;,
  0.656250;0.843750;,
  0.500000;0.850000;,
  0.500000;1.000000;,
  0.633368;0.788507;,
  0.610485;0.733265;,
  0.555243;0.710382;,
  0.500000;0.687500;,
  0.444757;0.710382;,
  0.389515;0.733265;,
  0.366632;0.788507;,
  0.343750;0.843750;,
  0.593750;0.687500;,
  0.406250;0.687500;,
  0.562500;0.687500;,
  0.531250;0.687500;,
  0.500000;0.687500;,
  0.468750;0.687500;,
  0.546875;0.687500;,
  0.578125;0.687500;,
  0.484375;0.687500;,
  0.515625;0.687500;,
  0.531250;0.558004;,
  0.500000;0.558004;,
  0.468750;0.558004;,
  0.406250;0.558004;,
  0.593750;0.558004;,
  0.562500;0.558004;,
  0.610485;0.045765;,
  0.500000;0.000000;,
  0.500000;0.000000;,
  0.610485;0.045765;,
  0.610485;0.266735;,
  0.656250;0.156250;,
  0.656250;0.156250;,
  0.610485;0.266735;,
  0.500000;0.312500;,
  0.500000;0.312500;,
  0.389515;0.266735;,
  0.389515;0.266735;,
  0.343750;0.156250;,
  0.343750;0.156250;,
  0.389515;0.045765;,
  0.389515;0.045765;,
  0.500000;0.000000;,
  0.610485;0.045765;,
  0.656250;0.156250;,
  0.610485;0.266735;,
  0.500000;0.312500;,
  0.389515;0.266735;,
  0.343750;0.156250;,
  0.389515;0.045765;,
  0.500000;0.000000;,
  0.610485;0.045765;,
  0.656250;0.156250;,
  0.610485;0.266735;,
  0.500000;0.312500;,
  0.389515;0.266735;,
  0.343750;0.156250;,
  0.389515;0.045765;;
 }
}