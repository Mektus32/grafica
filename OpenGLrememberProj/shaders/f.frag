varying vec2 texCoord;  // �������� ���������� ��������� 
// ������ ����� ���������
uniform vec3 color;

uniform float phase; //���� �������� �� ��������� ���� ���������

void main(void)
{
	float x=texCoord.x;
	float y=texCoord.y;
	float R=0.5;
	// �������� � ������ RGBA 
	if(R*R> (x-0.5)*(x-0.5)+(y-0.5)*(y-0.5))
	  gl_FragColor=vec4(color,1);
    
	if( abs((y-0.5)*5-sin(x*10+phase))<0.03 )
	   gl_FragColor=vec4(1,0,0,1);

	if(R*R < (x-0.5)*(x-0.5)+(y-0.5)*(y-0.5))
	  gl_FragColor=vec4(gl_FragColor.rgb,0);

}
