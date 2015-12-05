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
 123;
 -4.36944;-17.26434;4.36944;,
 -0.00000;-17.26434;6.17933;,
 0.00000;2.56164;6.27319;,
 -4.43582;2.56164;4.43582;,
 -0.00000;-17.26434;6.17933;,
 4.36944;-17.26434;4.36944;,
 4.43582;2.56164;4.43582;,
 0.00000;2.56164;6.27319;,
 6.17933;-17.26434;-0.00000;,
 6.27319;2.56164;0.00000;,
 4.36944;-17.26434;-4.36944;,
 4.43582;2.56164;-4.43582;,
 0.00000;-17.26434;-6.17933;,
 0.00000;2.56164;-6.27319;,
 -4.36944;-17.26434;-4.36944;,
 -4.43582;2.56164;-4.43582;,
 -6.17933;-17.26434;0.00000;,
 -6.27319;2.56164;0.00000;,
 -0.00000;-17.59725;2.30071;,
 -1.62685;-17.59725;1.62685;,
 0.00000;-17.59725;-0.00000;,
 1.62685;-17.59725;1.62685;,
 2.30071;-17.59725;-0.00000;,
 1.62685;-17.59725;-1.62685;,
 0.00000;-17.59725;-2.30071;,
 -1.62685;-17.59725;-1.62685;,
 -2.30071;-17.59725;0.00000;,
 -1.40207;24.41198;1.40206;,
 0.00000;24.41198;1.98281;,
 -0.00000;24.41196;-0.00001;,
 1.40207;24.41197;1.40206;,
 1.98283;24.41197;-0.00001;,
 1.40207;24.41197;-1.40208;,
 -0.00001;24.41197;-1.98284;,
 -1.40208;24.41196;-1.40208;,
 -1.98283;24.41197;-0.00002;,
 0.00000;6.71412;-5.87480;,
 -4.15411;6.71412;-4.15411;,
 -4.29157;15.29970;-4.29157;,
 0.00000;15.29970;-6.06919;,
 4.15411;6.71412;-4.15411;,
 4.29156;15.29970;-4.29156;,
 5.87480;6.71412;0.00000;,
 6.06919;15.29970;0.00000;,
 4.15411;6.71412;4.15411;,
 4.29156;15.29970;4.29156;,
 0.00000;6.71412;5.87480;,
 0.00000;15.29970;6.06919;,
 -4.15411;6.71412;4.15411;,
 0.00000;6.71412;5.87480;,
 0.00000;15.29970;6.06919;,
 -4.29157;15.29970;4.29156;,
 -5.87480;6.71412;0.00000;,
 -6.06919;15.29970;0.00000;,
 0.00000;4.36361;-4.12828;,
 -2.91913;4.36361;-2.91913;,
 2.91913;4.36361;-2.91913;,
 4.12828;4.36361;0.00000;,
 2.91913;4.36361;2.91913;,
 0.00000;4.36361;4.12828;,
 -2.91913;4.36361;2.91913;,
 0.00000;4.36361;4.12828;,
 -4.12828;4.36361;0.00000;,
 0.00000;18.34571;4.42158;,
 -3.12653;18.34571;3.12653;,
 0.00000;21.37455;1.98284;,
 -1.40208;21.37455;1.40208;,
 3.12653;18.34571;3.12653;,
 0.00000;18.34571;4.42158;,
 1.40208;21.37455;1.40207;,
 0.00000;21.37455;1.98284;,
 4.42158;18.34571;-0.00000;,
 1.98284;21.37455;-0.00000;,
 3.12653;18.34571;-3.12653;,
 1.40208;21.37455;-1.40208;,
 0.00000;18.34571;-4.42158;,
 0.00000;21.37455;-1.98284;,
 -3.12653;18.34571;-3.12653;,
 -1.40208;21.37454;-1.40208;,
 -4.42158;18.34571;-0.00000;,
 -1.98284;21.37455;-0.00000;,
 -1.40208;21.37455;1.40208;,
 0.00000;21.37455;1.98284;,
 0.00000;24.41198;1.98281;,
 -1.40207;24.41198;1.40206;,
 0.00000;21.37455;1.98284;,
 1.40208;21.37455;1.40207;,
 1.40207;24.41197;1.40206;,
 0.00000;24.41198;1.98281;,
 1.98284;21.37455;-0.00000;,
 1.98283;24.41197;-0.00001;,
 1.40208;21.37455;-1.40208;,
 1.40207;24.41197;-1.40208;,
 0.00000;21.37455;-1.98284;,
 -0.00001;24.41197;-1.98284;,
 -1.40208;21.37454;-1.40208;,
 -1.40208;24.41196;-1.40208;,
 -1.98284;21.37455;-0.00000;,
 -1.98283;24.41197;-0.00002;,
 -3.49234;-18.49890;3.49234;,
 -4.36944;-17.26434;4.36944;,
 -6.17933;-17.26434;0.00000;,
 -4.93891;-18.49890;0.00000;,
 -0.00000;-18.49890;4.93891;,
 -0.00000;-17.26434;6.17933;,
 3.49234;-18.49890;3.49234;,
 4.36944;-17.26434;4.36944;,
 4.93891;-18.49890;-0.00000;,
 6.17933;-17.26434;-0.00000;,
 3.49234;-18.49890;-3.49234;,
 4.36944;-17.26434;-4.36944;,
 0.00000;-18.49890;-4.93891;,
 0.00000;-17.26434;-6.17933;,
 -3.49234;-18.49890;-3.49234;,
 -4.36944;-17.26434;-4.36944;,
 -2.27002;-18.49890;2.27002;,
 -0.00000;-18.49890;3.21029;,
 2.27002;-18.49890;2.27002;,
 3.21029;-18.49890;-0.00000;,
 2.27002;-18.49890;-2.27002;,
 0.00000;-18.49890;-3.21029;,
 -2.27002;-18.49890;-2.27002;,
 -3.21029;-18.49890;0.00000;;
 
 96;
 4;0,1,2,3;,
 4;4,5,6,7;,
 4;5,8,9,6;,
 4;8,10,11,9;,
 4;10,12,13,11;,
 4;12,14,15,13;,
 4;15,14,16,17;,
 4;16,0,3,17;,
 3;18,19,20;,
 3;21,18,20;,
 3;22,21,20;,
 3;23,22,20;,
 3;24,23,20;,
 3;25,24,20;,
 3;26,25,20;,
 3;19,26,20;,
 3;27,28,29;,
 3;28,30,29;,
 3;30,31,29;,
 3;31,32,29;,
 3;32,33,29;,
 3;33,34,29;,
 3;34,35,29;,
 3;35,27,29;,
 4;36,37,38,39;,
 4;40,36,39,41;,
 4;42,40,41,43;,
 4;44,42,43,45;,
 4;46,44,45,47;,
 4;48,49,50,51;,
 4;52,48,51,53;,
 4;37,52,53,38;,
 4;54,55,37,36;,
 4;56,54,36,40;,
 4;57,56,40,42;,
 4;58,57,42,44;,
 4;59,58,44,46;,
 4;60,61,49,48;,
 4;62,60,48,52;,
 4;55,62,52,37;,
 4;13,15,55,54;,
 4;11,13,54,56;,
 4;9,11,56,57;,
 4;6,9,57,58;,
 4;7,6,58,59;,
 4;3,2,61,60;,
 4;17,3,60,62;,
 4;15,17,62,55;,
 4;51,50,63,64;,
 4;64,63,65,66;,
 4;47,45,67,68;,
 4;68,67,69,70;,
 4;45,43,71,67;,
 4;67,71,72,69;,
 4;43,41,73,71;,
 4;71,73,74,72;,
 4;41,39,75,73;,
 4;73,75,76,74;,
 4;39,38,77,75;,
 4;75,77,78,76;,
 4;38,53,79,77;,
 4;77,79,80,78;,
 4;53,51,64,79;,
 4;79,64,66,80;,
 4;81,82,83,84;,
 4;85,86,87,88;,
 4;86,89,90,87;,
 4;89,91,92,90;,
 4;91,93,94,92;,
 4;93,95,96,94;,
 4;95,97,98,96;,
 4;97,81,84,98;,
 4;99,100,101,102;,
 4;100,99,103,104;,
 4;104,103,105,106;,
 4;106,105,107,108;,
 4;108,107,109,110;,
 4;110,109,111,112;,
 4;112,111,113,114;,
 4;114,113,102,101;,
 4;103,99,115,116;,
 4;105,103,116,117;,
 4;107,105,117,118;,
 4;109,107,118,119;,
 4;111,109,119,120;,
 4;113,111,120,121;,
 4;102,113,121,122;,
 4;99,102,122,115;,
 4;116,115,19,18;,
 4;117,116,18,21;,
 4;118,117,21,22;,
 4;119,118,22,23;,
 4;120,119,23,24;,
 4;121,120,24,25;,
 4;122,121,25,26;,
 4;115,122,26,19;;
 
 MeshMaterialList {
  1;
  96;
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
   0.640000;0.640000;0.640000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "PET01.png";
   }
  }
 }
 MeshNormals {
  98;
  -0.687036;0.236564;0.687036;,
  0.000000;0.236564;0.971616;,
  0.687036;0.236564;0.687036;,
  0.971616;0.236564;-0.000000;,
  0.687036;0.236564;-0.687036;,
  0.000000;0.236564;-0.971616;,
  -0.687036;0.236565;-0.687036;,
  -0.971616;0.236565;-0.000000;,
  -0.667779;-0.328851;-0.667779;,
  0.000000;-0.328851;-0.944382;,
  0.667779;-0.328851;-0.667779;,
  0.944382;-0.328851;-0.000000;,
  0.667779;-0.328851;0.667779;,
  0.000000;-0.328851;0.944382;,
  -0.667779;-0.328851;0.667779;,
  -0.944382;-0.328851;0.000000;,
  -0.567575;-0.596420;-0.567575;,
  0.000000;-0.596420;-0.802673;,
  0.567575;-0.596420;-0.567575;,
  0.802673;-0.596420;-0.000000;,
  0.567575;-0.596420;0.567575;,
  0.000000;-0.596420;0.802673;,
  -0.567575;-0.596420;0.567575;,
  -0.802673;-0.596420;0.000000;,
  -0.638490;0.429723;-0.638490;,
  0.000000;0.429723;-0.902961;,
  0.638490;0.429723;-0.638490;,
  0.902961;0.429723;0.000000;,
  0.638490;0.429723;0.638490;,
  0.000000;0.429723;0.902961;,
  -0.638490;0.429723;0.638490;,
  -0.902961;0.429723;0.000000;,
  -0.588567;0.554237;0.588566;,
  -0.665638;0.337421;0.665638;,
  0.000000;0.554237;0.832359;,
  0.000001;0.337421;0.941354;,
  0.588567;0.554237;0.588566;,
  0.665638;0.337420;0.665637;,
  0.832359;0.554237;-0.000000;,
  0.941354;0.337419;-0.000000;,
  0.588566;0.554237;-0.588567;,
  0.665638;0.337419;-0.665639;,
  -0.000000;0.554238;-0.832359;,
  -0.000000;0.337418;-0.941355;,
  -0.588566;0.554237;-0.588567;,
  -0.665638;0.337418;-0.665639;,
  -0.832359;0.554237;-0.000001;,
  -0.941355;0.337419;-0.000001;,
  0.000002;1.000000;-0.000006;,
  0.000001;1.000000;-0.000006;,
  -0.000000;1.000000;-0.000003;,
  -0.000001;1.000000;-0.000005;,
  -0.000002;1.000000;-0.000001;,
  -0.000001;1.000000;0.000001;,
  -0.000003;1.000000;0.000002;,
  -0.000001;1.000000;-0.000001;,
  0.000002;1.000000;-0.000005;,
  -0.650144;-0.393224;0.650144;,
  -0.264289;-0.927525;0.264289;,
  0.000000;-0.393224;0.919443;,
  0.000000;-0.927525;0.373761;,
  0.650144;-0.393224;0.650144;,
  0.264289;-0.927525;0.264289;,
  0.919443;-0.393224;-0.000000;,
  0.373761;-0.927525;-0.000000;,
  0.650144;-0.393224;-0.650144;,
  0.264289;-0.927525;-0.264289;,
  0.000000;-0.393224;-0.919443;,
  0.000000;-0.927525;-0.373761;,
  -0.264289;-0.927525;-0.264289;,
  -0.650144;-0.393224;-0.650144;,
  -0.919443;-0.393224;0.000000;,
  -0.373761;-0.927525;0.000000;,
  0.000000;-0.927880;-0.372880;,
  0.263666;-0.927880;-0.263666;,
  -0.263666;-0.927880;-0.263666;,
  -0.372880;-0.927880;0.000000;,
  -0.263666;-0.927880;0.263666;,
  0.000000;-0.927880;0.372880;,
  0.263666;-0.927880;0.263666;,
  0.372880;-0.927880;-0.000000;,
  0.000000;-1.000000;-0.000000;,
  -0.454841;0.765662;-0.454841;,
  0.000000;0.765662;-0.643243;,
  0.454841;0.765662;-0.454841;,
  0.643243;0.765662;0.000000;,
  0.454841;0.765662;0.454841;,
  0.000000;0.765662;0.643243;,
  -0.454842;0.765662;0.454842;,
  -0.643243;0.765662;0.000000;,
  0.000002;0.000007;1.000000;,
  -0.707106;0.000006;0.707107;,
  0.707107;0.000005;0.707107;,
  1.000000;0.000002;0.000000;,
  0.707106;0.000001;-0.707107;,
  0.000000;-0.000001;-1.000000;,
  -0.707106;-0.000002;-0.707107;,
  -1.000000;0.000001;0.000000;;
  96;
  4;57,59,29,30;,
  4;59,61,28,29;,
  4;61,63,27,28;,
  4;63,65,26,27;,
  4;65,67,25,26;,
  4;67,70,24,25;,
  4;24,70,71,31;,
  4;71,57,30,31;,
  3;73,74,81;,
  3;75,73,81;,
  3;76,75,81;,
  3;77,76,81;,
  3;78,77,81;,
  3;79,78,81;,
  3;80,79,81;,
  3;74,80,81;,
  3;48,49,50;,
  3;49,51,50;,
  3;51,52,50;,
  3;52,53,50;,
  3;53,54,50;,
  3;54,55,50;,
  3;55,56,50;,
  3;56,48,50;,
  4;9,8,6,5;,
  4;10,9,5,4;,
  4;11,10,4,3;,
  4;12,11,3,2;,
  4;13,12,2,1;,
  4;14,13,1,0;,
  4;15,14,0,7;,
  4;8,15,7,6;,
  4;17,16,8,9;,
  4;18,17,9,10;,
  4;19,18,10,11;,
  4;20,19,11,12;,
  4;21,20,12,13;,
  4;22,21,13,14;,
  4;23,22,14,15;,
  4;16,23,15,8;,
  4;25,24,82,83;,
  4;26,25,83,84;,
  4;27,26,84,85;,
  4;28,27,85,86;,
  4;29,28,86,87;,
  4;30,29,87,88;,
  4;31,30,88,89;,
  4;24,31,89,82;,
  4;0,1,34,32;,
  4;32,34,35,33;,
  4;1,2,36,34;,
  4;34,36,37,35;,
  4;2,3,38,36;,
  4;36,38,39,37;,
  4;3,4,40,38;,
  4;38,40,41,39;,
  4;4,5,42,40;,
  4;40,42,43,41;,
  4;5,6,44,42;,
  4;42,44,45,43;,
  4;6,7,46,44;,
  4;44,46,47,45;,
  4;7,0,32,46;,
  4;46,32,33,47;,
  4;33,35,90,91;,
  4;35,37,92,90;,
  4;37,39,93,92;,
  4;39,41,94,93;,
  4;41,43,95,94;,
  4;43,45,96,95;,
  4;45,47,97,96;,
  4;47,33,91,97;,
  4;58,57,71,72;,
  4;57,58,60,59;,
  4;59,60,62,61;,
  4;61,62,64,63;,
  4;63,64,66,65;,
  4;65,66,68,67;,
  4;67,68,69,70;,
  4;70,69,72,71;,
  4;60,58,74,73;,
  4;62,60,73,75;,
  4;64,62,75,76;,
  4;66,64,76,77;,
  4;68,66,77,78;,
  4;69,68,78,79;,
  4;72,69,79,80;,
  4;58,72,80,74;,
  4;73,74,74,73;,
  4;75,73,73,75;,
  4;76,75,75,76;,
  4;77,76,76,77;,
  4;78,77,77,78;,
  4;79,78,78,79;,
  4;80,79,79,80;,
  4;74,80,80,74;;
 }
 MeshTextureCoords {
  123;
  0.576974;0.976443;,
  0.653934;0.976443;,
  0.653934;0.660528;,
  0.576974;0.660528;,
  0.038248;0.976443;,
  0.115209;0.976443;,
  0.115209;0.660528;,
  0.038248;0.660528;,
  0.192170;0.976443;,
  0.192170;0.660528;,
  0.269130;0.976443;,
  0.269130;0.660528;,
  0.346091;0.976443;,
  0.346091;0.660528;,
  0.423052;0.976443;,
  0.423052;0.660528;,
  0.500013;0.976443;,
  0.500013;0.660528;,
  0.827918;0.774751;,
  0.790262;0.790349;,
  0.827918;0.828005;,
  0.865574;0.790349;,
  0.881172;0.828005;,
  0.865574;0.865661;,
  0.827918;0.881258;,
  0.790262;0.865661;,
  0.774665;0.828005;,
  0.851869;0.449836;,
  0.819230;0.436317;,
  0.819230;0.482474;,
  0.786592;0.449836;,
  0.773073;0.482474;,
  0.786592;0.515112;,
  0.819231;0.528632;,
  0.851869;0.515112;,
  0.865388;0.482474;,
  0.346091;0.594361;,
  0.423052;0.594361;,
  0.423052;0.457556;,
  0.346091;0.457556;,
  0.269130;0.594361;,
  0.269130;0.457556;,
  0.192170;0.594361;,
  0.192170;0.457556;,
  0.115209;0.594361;,
  0.115209;0.457556;,
  0.038248;0.594361;,
  0.038248;0.457556;,
  0.576974;0.594361;,
  0.653934;0.594361;,
  0.653934;0.457556;,
  0.576974;0.457556;,
  0.500013;0.594361;,
  0.500013;0.457556;,
  0.346091;0.631815;,
  0.423052;0.631815;,
  0.269130;0.631815;,
  0.192170;0.631815;,
  0.115209;0.631815;,
  0.038248;0.631815;,
  0.576974;0.631815;,
  0.653934;0.631815;,
  0.500013;0.631815;,
  0.653934;0.409019;,
  0.576974;0.409019;,
  0.653934;0.360757;,
  0.576974;0.360757;,
  0.115209;0.409019;,
  0.038248;0.409019;,
  0.115209;0.360757;,
  0.038248;0.360757;,
  0.192170;0.409019;,
  0.192170;0.360757;,
  0.269130;0.409019;,
  0.269130;0.360757;,
  0.346091;0.409019;,
  0.346091;0.360757;,
  0.423052;0.409019;,
  0.423052;0.360757;,
  0.500013;0.409019;,
  0.500013;0.360757;,
  0.907327;0.597376;,
  0.933850;0.597376;,
  0.933850;0.542952;,
  0.907327;0.542952;,
  0.721670;0.597376;,
  0.748192;0.597376;,
  0.748193;0.542952;,
  0.721670;0.542952;,
  0.774715;0.597376;,
  0.774715;0.542952;,
  0.801237;0.597376;,
  0.801237;0.542952;,
  0.827760;0.597376;,
  0.827760;0.542952;,
  0.854282;0.597376;,
  0.854282;0.542952;,
  0.880805;0.597376;,
  0.880804;0.542952;,
  0.747083;0.747169;,
  0.726781;0.726867;,
  0.684888;0.828005;,
  0.713600;0.828005;,
  0.827918;0.713686;,
  0.827918;0.684974;,
  0.908754;0.747169;,
  0.929056;0.726867;,
  0.942237;0.828005;,
  0.970949;0.828005;,
  0.908754;0.908840;,
  0.929056;0.929142;,
  0.827918;0.942323;,
  0.827918;0.971035;,
  0.747083;0.908840;,
  0.726781;0.929142;,
  0.775375;0.775461;,
  0.827918;0.753697;,
  0.880461;0.775461;,
  0.902225;0.828005;,
  0.880461;0.880548;,
  0.827918;0.902312;,
  0.775375;0.880548;,
  0.753611;0.828005;;
 }
}
