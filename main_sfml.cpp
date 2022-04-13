//#pragma comment(lib, "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\SFML-2.5.1\lib")
//подключаем библиотеки
#include <SFML/Graphics.hpp>
#pragma comment (lib, "opengl32.lib")
#pragma comment (lib, "glu32.lib")
//#pragma comment (lib, "glaux.lib")
#include <SFML/OpenGL.hpp>
#include <gl/GLU.h>//GLAUX нету GLUT нету, но SFML заменяет!!!
#include <vector>
// указываем путь к библиотекам
#pragma comment(lib,"sfml-graphics-d.lib")
#pragma comment(lib,"sfml-window-d.lib")
#pragma comment(lib,"sfml-system-d.lib")
#pragma comment(lib,"sfml-main-d.lib")

//задаем глобальные переменные
float xe = 0, ye = 1, ze = 3,//координаты камеры
xev = 0, yev = 0, zev = 0,//координаты вектора зрения - вектор от точки камеры к точке объекта на который смотрит камера
						  //будем использовать при повороте вверх-вниз и при движении вперед назад
//xo = 0, yo = 0, zo = 0,//координаты объекта на который смотрит камера - будем рассчитывать как координаты камеры + координаты вектора зрения
xed = 0, zed = -0.05; //координаты вектора движения - похож на вектор зрения но он всегда параллелен проскости XZ (шахматной доске)
					  //будем использовать при шаге влево-вправо
int angley = 0;      //угол поворота вверх-вниз


bool running = true;
const INT w = 1900, h = 980;
sf::RenderWindow window(sf::VideoMode(w, h), "C++ SFML OpenGL gluLookAt", sf::Style::Default, sf::ContextSettings(32, 8, 4, 4, 6));//задаем окно SFML с параметрами OpenGL

//класс шар
struct shar {
	float x = (rand() % 800 - 400) / 200.0, y = (rand() % 80 + 40) / 100.0, z = (rand() % 800 - 800) / 200.0;
	float r = (rand() % 100) / 100.0, g = (rand() % 100) / 100.0, b = (rand() % 100) / 100.0;
	int lineCount = 32;
	float radius = 0.1;
	GLUquadricObj* qobj = gluNewQuadric();

	void draw() {//функция рисования одного шара
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
//класс звездное небо
struct SKY {
	float x = 0, y = 0, z = 0;
	float r = (rand() % 100) / 100.0, g = (rand() % 100) / 100.0, b = (rand() % 100) / 100.0;
	int lineCount = (rand() % 8 + 8);
	float radius = (rand() % 1 + 100);
	GLUquadricObj* qobj = gluNewQuadric();

	void draw() {//функция рисования одного шара
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

//функция перерисовки окна и переустановкы камеры
void myreshape(int w = window.getSize().x, int h = window.getSize().y)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(60.0, (double)window.getSize().x / window.getSize().y, 0.1, 400);//если хотим включить проекцию
	//glOrtho(-20, 20, -20, 20, 50, -50);//вместо gluPerspective включить glOrtho - Если хотим задать паралельный  3D куб - но gluLookAt не работает по чемуто((

	//вычисляем вектор зрения
	float R = sqrt(xed * xed + zed * zed);
	xev = xed * cos(angley * 3.141592 / 180);
	yev = R * sin(angley * 3.141592 / 180);
	zev = zed * cos(angley * 3.141592 / 180);

	gluLookAt(xe, ye, ze, xe+xev, ye+yev, ze+zev, 0.0, 1.0, 0.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

//отработка событий клавиатуры
void keyboardControl(sf::Event event) {
	if (event.type == sf::Event::KeyPressed) {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {//вперед
			//расчет координат камеры при шаге вперед
			xe += xev;
			ye += yev;
			ze += zev;
			myreshape();
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {//шаг влево
			//расчет координат камеры при шаге влево
			xe += xed * cos(-90 * 3.141592 / 180) - zed * sin(-90 * 3.141592 / 180);
			ze += xed * sin(-90 * 3.141592 / 180) + zed * cos(-90 * 3.141592 / 180);
			myreshape();
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {//назад
			//расчет координат камеры при шаге назад
			xe -= xev;
			ye -= yev;
			ze -= zev;
			myreshape();
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {//шаг аправо
			//расчет координат камеры при шаге вправо
			xe += xed * cos(90 * 3.141592 / 180) - zed * sin(90 * 3.141592 / 180);
			ze += xed * sin(90 * 3.141592 / 180) + zed * cos(90 * 3.141592 / 180);
			myreshape();
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {//тупо вертикально вверх
			ye += 0.05;
			myreshape();
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {//тупо вертикально вниз
			ye -= 0.05;
			myreshape();
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))//выход
			running = false;

	}
}

//отработка событий мышки
void mouseControl(sf::Event event) {
	if (event.type == sf::Event::MouseMoved) {
		static int oldmx, oldmy;
		int mx = event.mouseMove.x, my = event.mouseMove.y;
		int mdx = mx - oldmx, mdy = my - oldmy;

		int n = 2; //угол поворота за один цикл 
		(mdx < -n) ? mdx = -n : ((mdx > n) ? mdx = n : mdx = 0);
		(mdy < -n) ? mdy = -n : ((mdy > n) ? mdy = n : mdy = 0);
		//Расчет вектора движения для поворот камеры влево или вправо. 
		float xt = xed;
		xed = xed * cos(mdx * 3.141592 / 180) - zed * sin(mdx * 3.141592 / 180);
		zed = xt * sin(mdx * 3.141592 / 180) + zed * cos(mdx * 3.141592 / 180);
		//запуск перерисовку окна и переустановку камеры
		angley -= mdy;
		angley %= 360;
		myreshape();
		oldmx = mx; oldmy = my;
	}
	//возвращаем курсор в центр если он достиг края окна
	if (sf::Mouse::getPosition().x <= 1 || sf::Mouse::getPosition().x >= window.getSize().x - 1 || 
		sf::Mouse::getPosition().y <= 1 || sf::Mouse::getPosition().y >= window.getSize().y - 1)
		sf::Mouse::setPosition(sf::Vector2i(window.getSize().x / 2, window.getSize().y / 2));
}

// функция рисования половины шахматной доски
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

// main() - функция, которая запускается первой
int main()
{
	//Начальные настройки 
	
	window.setVerticalSyncEnabled(true);
	window.setPosition({ 0,0 });
    //Начальные настройки OpenGL
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_ALPHA);
	glEnable(GL_BLEND);
	glMatrixMode(GL_PROJECTION);
	//glClearColor(0.2f, 0.2f, 0.2f, 1.0f); //установка цвета фона
	glClearColor(0, 0, 0, 0); //установка цвета фона
	ShowCursor(false);//убираем курсор
    
	srand(time(NULL));
	std::vector<shar> shars(10);//генерируем шары
	std::vector<SKY> skys(100);//генерируем небо

	sf::Mouse::setPosition(sf::Vector2i(window.getSize().x / 2, window.getSize().y / 2));
	int countW = 0;
    
	// цикл отрисовки экрана для создания динамической сцены
	while (running) 
	{
		glMatrixMode(GL_PROJECTION);//работаем с матрицей проекции(с системой координат)
		sf::Event event;
		while (window.pollEvent(event))//обработка событий
		{
			if (event.type == sf::Event::Closed)
				running = false;
			if (event.type == sf::Event::Resized)
				myreshape(window.getSize().x, window.getSize().y);
			
			keyboardControl(event);
			mouseControl(event);
		}
		
		glMatrixMode(GL_MODELVIEW);//работаем с матрицей модели(объектов сцены)
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//очищаем буфер цвета и глубины
		
		//рисуем шахматную доску
		glPushMatrix();
		glScalef(2, 2, 2);
		draw8Quads();
		glTranslatef(0, 0, -1);
		draw8Quads();
		glPopMatrix();


		for (auto& sh : shars) {//рисуем шары
			//меняем параметр lineCount шара на 4 если его касаемся
			if (((xe  - sh.x) * (xe  - sh.x) + (ye  - sh.y) * (ye  - sh.y) + (ze  - sh.z) * (ze  - sh.z) <= sh.radius * sh.radius*4) || ((xe + xev * 5 - sh.x) * (xe + xev * 5 - sh.x) + (ye + yev * 5 - sh.y) * (ye + yev * 5 - sh.y) + (ze + zev * 5 - sh.z) * (ze + zev * 5 - sh.z) <= sh.radius * sh.radius*4) ) {
				sh.lineCount = 4;
				//sh.r = 1; sh.g = 0; sh.b = 0;
			}
			//меняем цвет шара если параметр lineCount шара = 4 
			if (sh.lineCount == 4 && countW % 10 == 0)
			{
				//float rtmp = sh.r;  sh.r = sh.g; sh.g = sh.b; sh.b = rtmp;
				sh.r = (rand() % 100) / 100.0, sh.g = (rand() % 100) / 100.0, sh.b = (rand() % 100) / 100.0;
			}

			sh.draw();//рисуем шары
		}
		
		for (auto& sk : skys) {//рисуем небо
			sk.draw();//рисуем небо
		}

		window.display();
		countW++;
	}

	return 0;
}