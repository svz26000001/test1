//#pragma comment(lib, "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\SFML-2.5.1\lib")
//���������� ����������
#include <SFML/Graphics.hpp>
#pragma comment (lib, "opengl32.lib")
#pragma comment (lib, "glu32.lib")
//#pragma comment (lib, "glaux.lib")
#include <SFML/OpenGL.hpp>
#include <gl/GLU.h>//GLAUX ���� GLUT ����, �� SFML ��������!!!
#include <vector>
// ��������� ���� � �����������
#pragma comment(lib,"sfml-graphics-d.lib")
#pragma comment(lib,"sfml-window-d.lib")
#pragma comment(lib,"sfml-system-d.lib")
#pragma comment(lib,"sfml-main-d.lib")

//������ ���������� ����������
float xe = 0, ye = 1, ze = 3,//���������� ������
xev = 0, yev = 0, zev = 0,//���������� ������� ������ - ������ �� ����� ������ � ����� ������� �� ������� ������� ������
						  //����� ������������ ��� �������� �����-���� � ��� �������� ������ �����
//xo = 0, yo = 0, zo = 0,//���������� ������� �� ������� ������� ������ - ����� ������������ ��� ���������� ������ + ���������� ������� ������
xed = 0, zed = -0.05; //���������� ������� �������� - ����� �� ������ ������ �� �� ������ ���������� ��������� XZ (��������� �����)
					  //����� ������������ ��� ���� �����-������
int angley = 0;      //���� �������� �����-����


bool running = true;
const INT w = 1900, h = 980;
sf::RenderWindow window(sf::VideoMode(w, h), "C++ SFML OpenGL gluLookAt", sf::Style::Default, sf::ContextSettings(32, 8, 4, 4, 6));//������ ���� SFML � ����������� OpenGL

//����� ���
struct shar {
	float x = (rand() % 800 - 400) / 200.0, y = (rand() % 80 + 40) / 100.0, z = (rand() % 800 - 800) / 200.0;
	float r = (rand() % 100) / 100.0, g = (rand() % 100) / 100.0, b = (rand() % 100) / 100.0;
	int lineCount = 32;
	float radius = 0.1;
	GLUquadricObj* qobj = gluNewQuadric();

	void draw() {//������� ��������� ������ ����
		glColor3f(r, g, b);
		gluQuadricDrawStyle(qobj, GLU_LINE);
		glPushMatrix();
		glLineWidth(1);
		glTranslatef(x, y, z);
		glScalef(2, 2, 2);
		gluSphere(qobj, radius, lineCount, lineCount);
		glPopMatrix();
	}
};
//����� �������� ����
struct SKY {
	float x = 0, y = 0, z = 0;
	float r = (rand() % 100) / 100.0, g = (rand() % 100) / 100.0, b = (rand() % 100) / 100.0;
	int lineCount = (rand() % 8 + 8);
	float radius = (rand() % 1 + 100);
	GLUquadricObj* qobj = gluNewQuadric();

	void draw() {//������� ��������� ������ ����
		glColor3f(r, g, b);
		gluQuadricDrawStyle(qobj, GLU_POINT);
		glPushMatrix();
		glRotatef(r * 360, r, g, b);
		//glTranslatef(x, y, z);
		glScalef(2, 2, 2);
		glPointSize(2);
		gluSphere(qobj, radius, lineCount, lineCount);
		glPopMatrix();
	}
};

//������� ����������� ���� � ������������� ������
void myreshape(int w = window.getSize().x, int h = window.getSize().y)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(60.0, (double)window.getSize().x / window.getSize().y, 0.1, 400);//���� ����� �������� ��������
	//glOrtho(-20, 20, -20, 20, 50, -50);//������ gluPerspective �������� glOrtho - ���� ����� ������ �����������  3D ��� - �� gluLookAt �� �������� �� ������((

	//��������� ������ ������
	float R = sqrt(xed * xed + zed * zed);
	xev = xed * cos(angley * 3.141592 / 180);
	yev = R * sin(angley * 3.141592 / 180);
	zev = zed * cos(angley * 3.141592 / 180);

	gluLookAt(xe, ye, ze, xe+xev, ye+yev, ze+zev, 0.0, 1.0, 0.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

//��������� ������� ����������
void keyboardControl(sf::Event event) {
	if (event.type == sf::Event::KeyPressed) {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {//������
			//������ ��������� ������ ��� ���� ������
			xe += xev;
			ye += yev;
			ze += zev;
			myreshape();
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {//��� �����
			//������ ��������� ������ ��� ���� �����
			xe += xed * cos(-90 * 3.141592 / 180) - zed * sin(-90 * 3.141592 / 180);
			ze += xed * sin(-90 * 3.141592 / 180) + zed * cos(-90 * 3.141592 / 180);
			myreshape();
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {//�����
			//������ ��������� ������ ��� ���� �����
			xe -= xev;
			ye -= yev;
			ze -= zev;
			myreshape();
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {//��� ������
			//������ ��������� ������ ��� ���� ������
			xe += xed * cos(90 * 3.141592 / 180) - zed * sin(90 * 3.141592 / 180);
			ze += xed * sin(90 * 3.141592 / 180) + zed * cos(90 * 3.141592 / 180);
			myreshape();
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {//���� ����������� �����
			ye += 0.05;
			myreshape();
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {//���� ����������� ����
			ye -= 0.05;
			myreshape();
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))//�����
			running = false;

	}
}

//��������� ������� �����
void mouseControl(sf::Event event) {
	if (event.type == sf::Event::MouseMoved) {
		static int oldmx, oldmy;
		int mx = event.mouseMove.x, my = event.mouseMove.y;
		int mdx = mx - oldmx, mdy = my - oldmy;

		int n = 2; //���� �������� �� ���� ���� 
		(mdx < -n) ? mdx = -n : ((mdx > n) ? mdx = n : mdx = 0);
		(mdy < -n) ? mdy = -n : ((mdy > n) ? mdy = n : mdy = 0);
		//������ ������� �������� ��� ������� ������ ����� ��� ������. 
		float xt = xed;
		xed = xed * cos(mdx * 3.141592 / 180) - zed * sin(mdx * 3.141592 / 180);
		zed = xt * sin(mdx * 3.141592 / 180) + zed * cos(mdx * 3.141592 / 180);
		//������ ����������� ���� � ������������� ������
		angley -= mdy;
		angley %= 360;
		myreshape();
		oldmx = mx; oldmy = my;
	}
	//���������� ������ � ����� ���� �� ������ ���� ����
	if (sf::Mouse::getPosition().x <= 1 || sf::Mouse::getPosition().x >= window.getSize().x - 1 || 
		sf::Mouse::getPosition().y <= 1 || sf::Mouse::getPosition().y >= window.getSize().y - 1)
		sf::Mouse::setPosition(sf::Vector2i(window.getSize().x / 2, window.getSize().y / 2));
}

// ������� ��������� �������� ��������� �����
void draw8Quads() {
	glBegin(GL_QUADS);
	//black
	glColor4f(0.35, 0.19, 0, 1);

	glVertex3f(-1, 0, -1);
	glVertex3f(-0.5, 0, -1);
	glVertex3f(-0.5, 0, -0.5);
	glVertex3f(-1, 0, -0.5);

	glVertex3f(-0.5, 0, -0.5);
	glVertex3f(0, 0, -0.5);
	glVertex3f(0, 0, 0);
	glVertex3f(-0.5, 0, 0);

	glVertex3f(0, 0, -1);
	glVertex3f(0.5, 0, -1);
	glVertex3f(0.5, 0, -0.5);
	glVertex3f(0, 0, -0.5);

	glVertex3f(0.5, 0, -0.5);
	glVertex3f(1, 0, -0.5);
	glVertex3f(1, 0, 0);
	glVertex3f(0.5, 0, 0);

	//white
	glColor4f(1, 0.9, 0.6, 1);

	glVertex3f(-1, 0, -0.5);
	glVertex3f(-0.5, 0, -0.5);
	glVertex3f(-0.5, 0, 0);
	glVertex3f(-1, 0, 0);

	glVertex3f(-0.5, 0, -1);
	glVertex3f(0, 0, -1);
	glVertex3f(0, 0, -0.5);
	glVertex3f(-0.5, 0, -0.5);

	glVertex3f(0, 0, -0.5);
	glVertex3f(0.5, 0, -0.5);
	glVertex3f(0.5, 0, 0);
	glVertex3f(0, 0, 0);

	glVertex3f(0.5, 0, -1);
	glVertex3f(1, 0, -1);
	glVertex3f(1, 0, -0.5);
	glVertex3f(0.5, 0, -0.5);

	glEnd();
}	 

// main() - �������, ������� ����������� ������
int main()
{
	//��������� ��������� 
	
	window.setVerticalSyncEnabled(true);
	window.setPosition({ 0,0 });
    //��������� ��������� OpenGL
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_ALPHA);
	glEnable(GL_BLEND);
	glMatrixMode(GL_PROJECTION);
	//glClearColor(0.2f, 0.2f, 0.2f, 1.0f); //��������� ����� ����
	glClearColor(0, 0, 0, 0); //��������� ����� ����
	ShowCursor(false);//������� ������
    
	srand(time(NULL));
	std::vector<shar> shars(10);//���������� ����
	std::vector<SKY> skys(100);//���������� ����

	sf::Mouse::setPosition(sf::Vector2i(window.getSize().x / 2, window.getSize().y / 2));
	int countW = 0;
    
	// ���� ��������� ������ ��� �������� ������������ �����
	while (running) 
	{
		glMatrixMode(GL_PROJECTION);//�������� � �������� ��������(� �������� ���������)
		sf::Event event;
		while (window.pollEvent(event))//��������� �������
		{
			if (event.type == sf::Event::Closed)
				running = false;
			if (event.type == sf::Event::Resized)
				myreshape(window.getSize().x, window.getSize().y);
			
			keyboardControl(event);
			mouseControl(event);
		}
		
		glMatrixMode(GL_MODELVIEW);//�������� � �������� ������(�������� �����)
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//������� ����� ����� � �������
		
		//������ ��������� �����
		glPushMatrix();
		glScalef(2, 2, 2);
		draw8Quads();
		glTranslatef(0, 0, -1);
		draw8Quads();
		glPopMatrix();


		for (auto& sh : shars) {//������ ����
			//������ �������� lineCount ���� �� 4 ���� ��� ��������
			if (((xe  - sh.x) * (xe  - sh.x) + (ye  - sh.y) * (ye  - sh.y) + (ze  - sh.z) * (ze  - sh.z) <= sh.radius * sh.radius*4) || ((xe + xev * 5 - sh.x) * (xe + xev * 5 - sh.x) + (ye + yev * 5 - sh.y) * (ye + yev * 5 - sh.y) + (ze + zev * 5 - sh.z) * (ze + zev * 5 - sh.z) <= sh.radius * sh.radius*4) ) {
				sh.lineCount = 4;
				//sh.r = 1; sh.g = 0; sh.b = 0;
			}
			//������ ���� ���� ���� �������� lineCount ���� = 4 
			if (sh.lineCount == 4 && countW % 10 == 0)
			{
				//float rtmp = sh.r;  sh.r = sh.g; sh.g = sh.b; sh.b = rtmp;
				sh.r = (rand() % 100) / 100.0, sh.g = (rand() % 100) / 100.0, sh.b = (rand() % 100) / 100.0;
			}

			sh.draw();//������ ����
		}
		
		for (auto& sk : skys) {//������ ����
			sk.draw();//������ ����
		}

		window.display();
		countW++;
	}

	return 0;
}