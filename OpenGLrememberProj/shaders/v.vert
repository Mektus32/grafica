varying vec2 texCoord; 
varying vec3 Normal;
varying vec3 Position;

void main(void) 
{     
	   
	gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix*gl_Vertex;   //�������� ������� �������� �� ������� ������� � �� ���������� �����
	Position = gl_Vertex.xyz;
	gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;   
	texCoord = gl_TexCoord[0].xy;   //��������� ��������� ���������� � ������
	Normal = normalize(gl_Normal); /*gl_NormalMatrix*/
}