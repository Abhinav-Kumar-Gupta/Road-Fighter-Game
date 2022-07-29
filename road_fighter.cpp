#include<stdlib.h>
#include<stdio.h>
#include<stdbool.h>
#ifdef __WIN64
    #include<windows.h>
    #include<GL/glut.h>
#elif __WIN32
    #include<windows.h>
    #include<GL/glut.h>
#elif _APPLE_CC_
    #include<GLUT/glut.h>
#elif __linux
    #include<GL/glut.h>
#elif __unix
    #include<GL/glut.h>
#endif
#include<limits.h>
#include<unistd.h>
#include<time.h>
#include<math.h>
#include<cstring>
#include<iostream>
#include<vector>
#include<MMSystem.h>

#define MAX_CARS 3

void drawCar();
void drawDivider();
void drawTree(int x, int y);
void drawBushes(int x, int y);
void drawSurroundings();
void stopGame();
void drawOtherCars(int);
void drawOther();
void setCars();
void moveOtherCars();
int detectColiision();
void draw_string(std::string);
void drawMainMenu();
void drawExitMenu();
void resumeGame();
void drawScore(int);
void drawDistanceBar();
void drawEnd();
void moveEnd();
void drawFuelBar();
void fuelMessage();
void drawFuelCar();

int foot_y = 200;
int tree_y = 200;
int div_y = 220;
int end_y = 200;

int a =0;
int a2=0;
int speed = 1;
int steerSpeed = 1;
int temp = 0;
float score = 0;
float distance = 178;
float fuel = 178;

int game_state = 0;

void moveDivider();
void moveRoad();
void moveCar();
void periodicFunction(int);
void moveFuelCar();

int lane[MAX_CARS];
int pos[MAX_CARS];
int lane_pos[MAX_CARS];
int speeds[MAX_CARS];
int speedsTemp[MAX_CARS];

GLdouble width=1200, height=800;

bool carMoveLeft = false, carMoveRight = false, carMoveFast = false, carStopped = false, gameStopped = false, horn = false;
bool highlightStart = false, highlightExit = false;
bool reachedEnd = false, gameCompleted = false, fuelOver = false;
bool fuel_collison = false;

//car variables
int car_x = 0;
int car_y = -100;

int fuel_x = 20;
int fuel_y = -80;
int fuel_collison_x = 0;
int fuel_collison_y = 0;
int fuel_timer = 0;

void maindisp()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);

    switch(game_state)
    {
        case 1: glClearColor(0.5,0.5,0.5,0.0);
                drawSurroundings();
                drawDivider();
                drawCar();
                drawOther();
                drawDistanceBar();
                drawFuelBar();
                drawFuelCar();
                break;

        case 0: drawMainMenu();
        		drawDistanceBar();
        		drawFuelBar();
                break;

        case 2: glClearColor(0.5,0.5,0.5,0.0);
                drawSurroundings();
                drawDivider();
                if(gameCompleted)
                    drawEnd();

                if(fuelOver)
                    fuelMessage();

                drawCar();
                drawOther();
                drawDistanceBar();
                drawFuelBar();
                drawExitMenu();
                break;
    }

    glFlush();
    glutSwapBuffers();
}

void keyBoardFunc(int key, int x, int y)
{
    switch(key)
    {
        case GLUT_KEY_LEFT : carMoveLeft = true; carMoveRight = false; break;
        case GLUT_KEY_RIGHT : carMoveRight = true; carMoveLeft = false; break;
        case GLUT_KEY_UP : carMoveFast = true; break;
    }
}

void keyboard_up_func(int k, int x, int y)
{
    switch(k)
    {
        case GLUT_KEY_LEFT : carMoveLeft=false;
                             break;
        case GLUT_KEY_RIGHT : carMoveRight=false;
                              break;
        case GLUT_KEY_UP : carMoveFast=false; break;
        case GLUT_KEY_DOWN : carStopped=false; break;
    }
}

void normalKeyBoardFunc(unsigned char key, int x, int y)
{
    if(game_state == 1)
    {
        switch(key)
        {
            //horn
            case 'H':
            case 'h': horn = true; break;
        }
    }
    else if(game_state == 0)
    {
        if(key ==13)
        {
            setCars();
            game_state = 1;
        }
    }
    else if(game_state == 2)
    {
    	if(key == 13)
    	{
    		gameStopped = false;
            gameCompleted = false;
            reachedEnd = false;
            score = 0;
            distance = 178;
            fuel = 178;
            fuelOver = false;
            end_y = 160;
            setCars();
            game_state = 1;
            car_x = 0;
		}
	}
}

void mouse_func(int button, int state, int x, int y)
{
    switch(game_state)
    {
        case 0: if(button == GLUT_LEFT_BUTTON && state == GLUT_UP)
                {
                    if(x >= 630 && x <= 810 && y >= 320 && y <= 405)
                    {
                        setCars();
                        game_state = 1;
                    }
                    else if(x >= 630 && x <= 810 && y >= 320 && y <= 575)
                        exit(0);
                }
                break;

        case 2: if(button == GLUT_LEFT_BUTTON && state == GLUT_UP)
                {
                    if(x >= 630 && x <= 810 && y >= 320 && y <= 405)
                    {
                        gameStopped = false;
                        gameCompleted = false;
                        reachedEnd = false;
                        score = 0;
                        distance = 178;
                        fuel = 178;
                        fuelOver = false;
                        end_y = 160;
                        setCars();
                        game_state = 1;
                        car_x = 0;
                    }
                    else if(x >= 630 && x <= 810 && y >= 320 && y <= 575)
                        exit(0);
                }
                break;
    }
}

void mouse_hover(int x, int y)
{
    if(x >= 630 && x <= 810 && y >= 320 && y <= 405)
    {
        highlightStart = true;
        highlightExit = false;
    }
    else if(x >= 630 && x <= 810 && y >= 320 && y <= 575)
    {
        highlightStart = false;
        highlightExit = true;
    }
    else
    {
        highlightStart = false;
        highlightExit = false;
    }
}

int main(int argc, char *argv[])
{
	
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize((int)width, (int)height);
    glutCreateWindow("Galaxian");
    glutFullScreen();

    gluOrtho2D(-240.0, 240.0, -160.0, 160);

    glutDisplayFunc(maindisp);
    glutTimerFunc(100, periodicFunction, 0);

    glutSpecialFunc(keyBoardFunc);
    glutSpecialUpFunc(keyboard_up_func);
    glutKeyboardFunc(normalKeyBoardFunc);
    glutMouseFunc(mouse_func);
    glutPassiveMotionFunc(mouse_hover);

    glutMainLoop();
    return 0;
}

//function draws a car at (car_x, car_y)
void drawCar(void)
{
    glPushMatrix();

    glTranslated(car_x, car_y+15, 0.0);

    glColor3f(1.0, 0.0, 0.0);
	glRectf(-7, -19, 7, -12);
    glRectf(-5.5, -28, 5.5, -19);
    glRectf(-7, -35, 7.0, -28);

    glBegin(GL_POLYGON);
        glVertex2f(-7.0,-12.0);
        glVertex2f(-5.0,-7.0);
        glVertex2f(5.0,-7.0);
        glVertex2f(7.0,-12.0);
        glVertex2f(-7.0,-12.0);
    glEnd();

	glColor3f(0.95, 0.95, 0.95);
    glRectf(-7.0, -36.0, -5.5, -28.0);
    glRectf(7.0, -36.0, 5.5, -28.0);
    glRectf(-7.0, -19.0, -6.5, -14.0);
    glRectf(7.0, -19.0, 6.5, -14.0);
    
    glColor3f(0.2, 0.2, 0.2);
    glRectf(-4.5, -29.0, 4.5, -26.0);
    glRectf(-4.5, -19.0, 4.5, -17.0);
    glBegin(GL_POLYGON);
        glVertex2f(-4.5,-17.0);
        glVertex2f(-3.5,-16.0);
        glVertex2f(3.5,-16.0);
        glVertex2f(4.5,-17.0);
        glVertex2f(-4.5,-17.0);
    glEnd();

    glPopMatrix();
}

void drawOtherCars(int i)
{
    glPushMatrix();

    glTranslated(lane_pos[i], pos[i], 0.0);

    switch(speeds[i])
    {
        case 4: glColor3f(0.9, 0.9, 0.0); break;
        case 0: glColor3f(0.0, 0.0, 0.0); break;
    }
    
	glRectf(-7, -19, 7, -12);
    glRectf(-5.5, -28, 5.5, -19);
    glRectf(-7, -35, 7.0, -28);

    glBegin(GL_POLYGON);
        glVertex2f(-7.0,-12.0);
        glVertex2f(-5.0,-7.0);
        glVertex2f(5.0,-7.0);
        glVertex2f(7.0,-12.0);
        glVertex2f(-7.0,-12.0);
    glEnd();

	glColor3f(0.95, 0.95, 0.95);
    glRectf(-7.0, -36.0, -5.5, -28.0);
    glRectf(7.0, -36.0, 5.5, -28.0);
    glRectf(-7.0, -19.0, -6.5, -14.0);
    glRectf(7.0, -19.0, 6.5, -14.0);
    
    glColor3f(0.2, 0.2, 0.2);
    glRectf(-4.5, -29.0, 4.5, -26.0);
    glRectf(-4.5, -19.0, 4.5, -17.0);
    glBegin(GL_POLYGON);
        glVertex2f(-4.5,-17.0);
        glVertex2f(-3.5,-16.0);
        glVertex2f(3.5,-16.0);
        glVertex2f(4.5,-17.0);
        glVertex2f(-4.5,-17.0);
    glEnd();

    glPopMatrix();
}

void drawTree(int x, int y)
{
	glPushMatrix();
	
    glTranslated(x, y, 0.0);
    glColor3f(0.20, 0.19, 0.18);
	glRectf(-1, -1, 2, 1);
    glBegin(GL_POLYGON);
        glVertex2f(2,1);
        glVertex2f(2,3);
        glVertex2f(4,9);
        glVertex2f(4,-9);
        glVertex2f(2,-3);
        glVertex2f(2,-1);
    glEnd();
    
    glRectf(4, -9, 8, 9);
    glRectf(8, -8, 11, 8);
    glRectf(11, -7, 13.5, 7);
    glRectf(13.5, -6, 15.5, 6);
    glRectf(15.5, -5, 17.5, 5);
    glRectf(17.5, -3.25, 19, 3.25);
    glRectf(19, -1.75, 20.5, 1.75);
    glRectf(20.5, -0.75, 21.5, 0.75);
    
    glColor3f(0.0, 0.2, 0.0);
    glBegin(GL_POLYGON);
        glVertex2f(0,0);
        glVertex2f(0,2);
        glVertex2f(-5,11);
        glVertex2f(-11,11);
        glVertex2f(-16,2);
        glVertex2f(-16,-2);
        glVertex2f(-11,-11);
        glVertex2f(-5,-11);
        glVertex2f(0,-2);
        glVertex2f(0,0);
    glEnd();

    glPopMatrix();
}

void drawBushes(int x, int y)
{
	glPushMatrix();
	
    glTranslated(x, y, 0.0);
    glColor3f(0.15, 0.14, 0.13);
    glBegin(GL_POLYGON);
        glVertex2f(2,1);
        glVertex2f(2,3);
        glVertex2f(4,9);
        glVertex2f(4,-9);
        glVertex2f(2,-3);
        glVertex2f(2,-1);
    glEnd();
    
    glRectf(4, -9, 8, 9);
    glRectf(8, -8, 11, 8);
    glRectf(11, -7, 13.5, 7);
    glRectf(13.5, -6, 15.5, 6);
    glRectf(15.5, -5, 17.5, 5);
    glRectf(17.5, -3.25, 19, 3.25);
    glRectf(19, -1.75, 20.5, 1.75);
    glRectf(20.5, -0.75, 21.5, 0.75);

    glPopMatrix();
}

void drawSurroundings()
{
	glColor3f(0.0, 0.75, 0.0);
    glRectd(63, -160, 240, 160);
    glRectd(-120, 160, -63, -160);
    
    glColor3f(0.0, 0.0, 0.0);
    glRectd(-120, 160, -240, -160);
    glRectd(115, -160, 240, 160);
    
    int i,y;
    glPushMatrix();
    //Draw both side foot path
    glTranslated(58, 0, 0);
    y = foot_y + 20;

    for(i=0; i<20; i++)
    {
        if(a==0)
        {
            if(i % 2 == 0)
                glColor3f(0.85, 0.85, 0.85);
            else
                glColor3f(0.15, 0.15, 0.15);
        }
        else
        {
            if(i % 2 == 1)
                glColor3f(0.85, 0.85, 0.85);
            else
                glColor3f(0.15, 0.15, 0.15);
        }
        y-=20;
        glRectd(5, y, -5, y-20);
        glRectd(5-116, y, -5-116, y-20);
    }
    glPopMatrix();
    
    
	glPushMatrix();
    y = tree_y + 40;
    bool flag;

    for(i=0; i<10; i++)
    {
        if(a2==0)
        {
            if(i % 2 == 0)
				flag = true;
            else
				flag = false;
        }
        else
        {
            if(i % 2 == 1)
				flag = true;
            else
				flag = false;
        }
        y-=40;
        if(flag)
        {
        	drawTree(-100,y);
        	drawTree(85,y-40);
        	drawBushes(-122,y-25);
		}
        
    }
    glPopMatrix();
}

void drawDivider()
{
    int i;
    int y = div_y + 80;
    glColor3f(0.96, 0.96, 0.96);

    for(i=0; i<8; i++)
    {
        y-=80;
        glRectd(-2, y, 2, y-40);
    }
}

void stopGame()
{
    speed = 0;
    steerSpeed = 1;
}

void resumeGame()
{
    speed = 2;
    steerSpeed = 1;
}

void drawOther()
{
    int i;
    for(i=0; i<MAX_CARS; i++)
    {
        drawOtherCars(i);
    }
}

void setCars()
{
    int i;
    for(i=0; i<MAX_CARS; i++)
    {
        switch(rand()%3)
        {
        	case 0:pos[i]=100;
        	break;
        	case 1:pos[i]=150;
        	break;
        	case 2:pos[i]=200;
        	break;	
		}
        
        switch(rand()%5)
        {
        	case 0:lane_pos[i]=-40;
        	break;
        	case 1:lane_pos[i]=-20;
        	break;
        	case 2:lane_pos[i]=20;
        	break;
        	case 3:lane_pos[i]=40;
        	break;
        	case 4:lane_pos[i]=0;
		}
		  speeds[i]=4;
    }
}

void moveDivider()
{
    if(game_state == 1)
    {
    	div_y -= speed;
	    if(div_y < 120 && distance > 0)
	    {
	        div_y = 200;
	        if(carMoveFast)
	        {
	            distance -= 1;
	            score += 15;
	            fuel -= 1.25;
	        }
	        else
	        {
	            distance -= 0.5;
	            score += 5;
	            fuel -= 1;
	        }
	        if(fuel < 0)
	            fuelOver = true;
	
	        if(distance < 5)
	            reachedEnd = true;
	    }
	}
}

void moveCar()
{
    if(game_state == 1)
    {
    	if(carMoveLeft)
	        car_x -= steerSpeed;
	
	    else if(carMoveRight)
	        car_x += steerSpeed;
	
	    if(car_x > 45 || car_x < -45)
	    {
	        game_state = 2;
	        gameStopped = true;
	    }
	}
}

void moveRoad()
{
    if(game_state == 1)
    {
    	foot_y -= speed;
	    tree_y -= speed/1.2;
	    if(tree_y < 160)
	    {
	        tree_y = 180;
	        if(a2 == 0)
	            a2 = 1;
	        else
	            a2 = 0;
	    }
	    if(foot_y < 160)
	    {
	        foot_y = 180;
	        if(a == 0)
	            a = 1;
	        else
	            a = 0;
	    }
	}
}

void periodicFunction(int v)
{
    if(gameStopped)
        stopGame();
    else
        resumeGame();

    if(speed != 0)
    {
        if(carMoveFast)
            speed = 6;
        else
            speed =2;
    }

    if(fuelOver)
    {
        gameStopped = true;
        game_state = 2;
    }

    moveRoad();
    moveDivider();
    moveCar();
    moveOtherCars();
    moveEnd();
    moveFuelCar();

    if(!detectColiision() && !gameStopped && game_state == 1)
        temp += 15;

    if(temp > 1000)
    {
        temp = 0;
    }

    glutPostRedisplay();
    glutTimerFunc(10, periodicFunction, v);
}

void moveOtherCars()
{
    int i;
    for(i=0; i<MAX_CARS; i++)
    {
        pos[i] += -speed + speeds[i];
        if(pos[i] < -200 || pos[i] > 301)
        {
            switch(rand()%3)
	        {
	        	case 0:pos[i]=200;
	        	break;
	        	case 1:pos[i]=250;
	        	break;
	        	case 2:pos[i]=300;
	        	break;	
			}            
            
            switch(rand()%5)
	        {
	        	case 0:lane_pos[i]=-40;
	        	break;
	        	case 1:lane_pos[i]=-20;
	        	break;
	        	case 2:lane_pos[i]=20;
	        	break;
	        	case 3:lane_pos[i]=40;
	        	break;
	        	case 4:lane_pos[i]=0;
			}
            speeds[i] = 4;
        }
    }

    if(horn)
    {
        speeds[(car_x + 60)/40]++;
        if(speeds[(car_x + 60)/40] > 7)
            speeds[(car_x + 60)/40] = 7;
        horn = false;
    }
}

int detectColiision()
{
    if(game_state != 1)
        return 0;

    int i, limit;
    for(i=0; i<MAX_CARS; i++)
    {
        if(pos[i] < -55 && pos[i] > -115)
        {
            limit = lane_pos[i];
            if(car_x < limit + 15 && car_x > limit -15)
            {
                speeds[i] = 0;
                gameStopped = true;
                game_state = 2;
                return 1;
            }
        }
    }

	if(abs(fuel_x-car_x)<=15)
    {
        if(fuel_y < -55 && fuel_y > -115)
        {
            fuel += 40;
            score += 1000;
            if(fuel > 178)
                fuel = 178;
		    
		    fuel_collison_x = fuel_x;
		    fuel_collison_y = fuel_y;
		    fuel_timer = 150;
		    
            fuel_y = 1000 + ((rand()%3)*50);
            switch(rand()%4)
	        {
	        	case 0:fuel_x=-40;
	        	break;
	        	case 1:fuel_x=20;
	        	break;
	        	case 2:fuel_x=40;
	        	break;
	        	case 3:fuel_x=-20;
	        	break;
			}
			fuel_collison = true;
        }
    }
    return 0;
}

void draw_string(std::string str)
{
    for(unsigned int i=0;i<str.length();i++)
    {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, *(str.begin()+i));
    }
}

void drawMainMenu()
{
    //Draw start button
    glClearColor(0.5, 0.5, 0.5, 0.0);
    glColor3f(0.0, 0.0, 0.0);
    drawSurroundings();
    drawDivider();
    drawCar();

    glColor3f(1.0, 1.0, 1.0);
    glPushMatrix();
    glTranslated(0, 30, 0);
    glBegin(GL_LINE_LOOP);
        glVertex2f(30, 15);
        glVertex2f(30, -15);
        glVertex2f(-30, -15);
        glVertex2d(-30, 15);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslated(-20, 30, 0);
    glScalef(0.1, 0.1, 0.1);
    glColor3f(1.0, 1.0, 1.0);
    draw_string("START");

    glPopMatrix();

    //draw exit button
    glColor3f(1.0, 1.0, 1.0);

    glPushMatrix();
    glTranslated(0, -30, 0);
    glBegin(GL_LINE_LOOP);
        glVertex2f(30, 15);
        glVertex2f(30, -15);
        glVertex2f(-30, -15);
        glVertex2d(-30, 15);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslated(-15, -30, 0);
    glScalef(0.1, 0.1, 0.1);
    draw_string("EXIT");

    glPopMatrix();

    if(highlightStart)
    {
        glColor3f(1.0, 0.0, 0.0);

        glPushMatrix();
        glTranslated(0, 30, 0);
        glBegin(GL_LINE_LOOP);
            glVertex2f(35, 20);
            glVertex2f(35, -20);
            glVertex2f(-35, -20);
            glVertex2d(-35, 20);
        glEnd();
        glPopMatrix();
    }

    if(highlightExit)
    {
        glColor3f(1.0, 0.0, 0.0);

        glPushMatrix();
        glTranslated(0, -30, 0);
        glBegin(GL_LINE_LOOP);
            glVertex2f(35, 20);
            glVertex2f(35, -20);
            glVertex2f(-35, -20);
            glVertex2d(-35, 20);
        glEnd();
        glPopMatrix();
    }
}

void drawExitMenu()
{
    //draw restart button
    glColor3f(1.0, 1.0, 1.0);
    glPushMatrix();
    glTranslated(0, 30, 0);
    glBegin(GL_LINE_LOOP);
        glVertex2f(30, 15);
        glVertex2f(30, -15);
        glVertex2f(-30, -15);
        glVertex2d(-30, 15);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslated(-25, 30, 0);
    glScalef(0.1, 0.1, 0.1);
    glColor3f(1.0, 1.0, 1.0);
    draw_string("RESTART");

    glPopMatrix();

    //draw exit button
    glColor3f(1.0, 1.0, 1.0);

    glPushMatrix();
    glTranslated(0, -30, 0);
    glBegin(GL_LINE_LOOP);
        glVertex2f(30, 15);
        glVertex2f(30, -15);
        glVertex2f(-30, -15);
        glVertex2d(-30, 15);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslated(-15, -30, 0);
    glScalef(0.1, 0.1, 0.1);
    draw_string("EXIT");

    glPopMatrix();

    if(highlightStart)
    {
        glColor3f(1.0, 0.0, 0.0);

        glPushMatrix();
        glTranslated(0, 30, 0);
        glBegin(GL_LINE_LOOP);
            glVertex2f(35, 20);
            glVertex2f(35, -20);
            glVertex2f(-35, -20);
            glVertex2d(-35, 20);
        glEnd();
        glPopMatrix();
    }

    if(highlightExit)
    {
        glColor3f(1.0, 0.0, 0.0);

        glPushMatrix();
        glTranslated(0, -30, 0);
        glBegin(GL_LINE_LOOP);
            glVertex2f(35, 20);
            glVertex2f(35, -20);
            glVertex2f(-35, -20);
            glVertex2d(-35, 20);
        glEnd();
        glPopMatrix();
    }
}

void drawScore(int score)
{
    int temp = score;
    int str[20],i=0;

    while(temp>0)
    {
        str[i++] = (temp%10);
        temp /= 10;
    }
    while (i != 6)
    {
    	str[i] = 0;
    	i++;
	}    
    i--;
    while(i>=0)
    {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, str[i--]+'0');
    }
}

void drawDistanceBar()
{
	glColor3f(0.6, 0.6, 0.6);
	glRectd(-130, -160, -150, 160);
	glColor3f(0.95, 0.95, 0.95);
	glRectd(-130, -160, -132, 160);
	glRectd(-152, -160, -150, 160);
	
	glColor3f(0.99, 0.99, 0.99);
	glRectd(-147, -152, -146, -140);
	glRectd(-147, 152, -146, 140);
	
	glColor3f(0.34, 0.99, 0.99);
	glBegin(GL_POLYGON);
        glVertex2f(-146,151);
        glVertex2f(-141,151);
        glVertex2f(-146,146);
        glVertex2f(-146,151);
    glEnd();
    
    glColor3f(0.9, 0.9, 0.0);
	glBegin(GL_POLYGON);
        glVertex2f(-146,-141);
        glVertex2f(-141,-141);
        glVertex2f(-146,-146);
        glVertex2f(-146,-141);
    glEnd();
    
	
    glPushMatrix();
    glColor3f(1.0, 1.0, 1.0);
    int temp = (178-distance)*100/183;
    glTranslated(-145, temp*3-154 , 0);
    glRectf(0, 0, 8, 4);
    glRectf(7, 4, 1, 10);
    glRectf(0, 10, 8, 15);
    glRectf(0.5, 15, 7.5, 16);
    glRectf(1, 16, 7, 17);
    glRectf(0, 0, 1, -1);
    glRectf(7, 0, 8, -1);
    
    glColor3f(0.9,0.0,0.0);
    glRectf(0.75, 1.0, 7.25, 4);
    glRectf(6.25, 4, 1.75, 10);
    glRectf(0.5, 10, 7.5, 15);
    glRectf(1, 15, 7, 16);
    glRectf(1.5, 16, 6.5, 16.5);
    glRectf(0.75, 0.75, 7.5, 4);
    
    glColor3f(0.1,0.1,0.1);
    glRectf(6.25, 4, 1.75, 7);
    glRectf(6.25, 10, 1.75, 12);
    glRectf(5.25, 12, 2.75, 13.25);
    glPopMatrix();
}

void drawFuelBar()
{
	//fuel
	glColor3f(1.0, 0.0, 0.0);
    glPushMatrix();
    glTranslated(135, -35, 0);
    glBegin(GL_LINE_LOOP);
        glVertex2f(34, 8);
        glVertex2f(34, -8);
        glVertex2f(0, -8);
        glVertex2d(0, 8);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslated(135, -40, 0);
    glScalef(0.1, 0.1, 0.1);
    glColor3f(1.0, 1.0, 1.0);
    draw_string("FUEL");
    glPopMatrix();

    glPushMatrix();
    glTranslated(140, -60, 0);
    glScalef(0.1, 0.1, 0.1);
    glColor3f(1.0, 1.0, 1.0);
    int temp = (fuel*100)/178;
    drawScore(temp);
    glPopMatrix();
    glColor3f(0.0, 0.0, 0.0);
    glRectd(140, -62, 163, -45);
    

    glPushMatrix();
    glTranslated(135, 100, 0);
    glScalef(0.1, 0.1, 0.1);
    glColor3f(1.0, 1.0, 1.0);
    draw_string("1P");
    glPopMatrix();

    glPushMatrix();
    glTranslated(135, 80, 0);
    glScalef(0.1, 0.1, 0.1);
    glColor3f(1.0, 1.0, 1.0);
    int temp2 = score/50;
    temp2 = temp2*50;
    drawScore(temp2);
    glPopMatrix();
}

void drawEnd()
{
    int i, j;
    for(i=0; i<5; i++)
    {
        if(i % 2 == 0)
            glColor3f(0.0, 0.0, 0.0);
        else
            glColor3f(1.0, 1.0, 1.0);
        glPushMatrix();

        glTranslated(-53, end_y + 10*i, 0);

        for(j=0; j<10; j++)
        {
            if(i%2)
            {
                if(j % 2 == 0)
                    glColor3f(0.0, 0.0, 0.0);
                else
                    glColor3f(1.0, 1.0, 1.0);
            }
            else
            {
                if(j % 2)
                    glColor3f(0.0, 0.0, 0.0);
                else
                    glColor3f(1.0, 1.0, 1.0);
            }
            glRectf(10.6 * j,0,10.6*(j+1),10.6);
        }
        glPopMatrix();
    }
}

void fuelMessage()
{
    glPushMatrix();
    glTranslated(125, 20, 0);
    glScalef(0.1,0.1,0.1);
    glColor3f(1.0, 0, 0);
    draw_string("Fuel Exhausted\n");
    glPopMatrix();
}

void moveEnd()
{
    if(reachedEnd)
        end_y-=speed;

    if(end_y < -125)
    {
        gameStopped = true;
        gameCompleted = true;
        game_state = 2;
    }
}

void drawFuelCar()
{
	glPushMatrix();

    glTranslated(fuel_x, fuel_y+15, 0.0);

    glColor3f(0.95, 0.0, 0.0);
	glRectf(-7, -19, 7, -12);
	glColor3f(0.9, 0.9, 0.0);
    glRectf(-5.5, -28, 0, -19);
    glRectf(-7, -35, 0, -28);
    glColor3f(0.34, 0.9, 0.9);
    glRectf(0, -28, 5.5, -19);
    glRectf(0, -35, 7.0, -28);

	glColor3f(0.95, 0.0, 0.0);
    glBegin(GL_POLYGON);
        glVertex2f(-7.0,-12.0);
        glVertex2f(-5.0,-7.0);
        glVertex2f(5.0,-7.0);
        glVertex2f(7.0,-12.0);
        glVertex2f(-7.0,-12.0);
    glEnd();

	glColor3f(0.95, 0.95, 0.95);
    glRectf(-7.0, -36.0, -5.5, -28.0);
    glRectf(7.0, -36.0, 5.5, -28.0);
    glRectf(-7.0, -19.0, -6.5, -14.0);
    glRectf(7.0, -19.0, 6.5, -14.0);
    
    glColor3f(0.2, 0.2, 0.2);
    glRectf(-4.5, -29.0, 4.5, -26.0);
    glRectf(-4.5, -19.0, 4.5, -17.0);
    glBegin(GL_POLYGON);
        glVertex2f(-4.5,-17.0);
        glVertex2f(-3.5,-16.0);
        glVertex2f(3.5,-16.0);
        glVertex2f(4.5,-17.0);
        glVertex2f(-4.5,-17.0);
    glEnd();

    glPopMatrix();
    
    if(fuel_collison == true)
    {
    	fuel_timer -= 4;
    	if(fuel_timer < 10)
    	{
    		fuel_collison = false;
		}
    	glPushMatrix();
		glTranslated(fuel_collison_x-15, fuel_collison_y, 0);
		glScalef(0.1, 0.1, 0.1);
		glColor3f(1.0, 1.0, 1.0);
		draw_string("1000");
		glPopMatrix();
	}
}

void moveFuelCar()
{
    fuel_y-=speed - 4;
    if(fuel_y < -200 || fuel_y > 1700)
    {
        fuel_y = 1000 + ((rand()%3)*50);
        switch(rand()%4)
	        {
	        	case 0:fuel_x=-40;
	        	break;
	        	case 1:fuel_x=20;
	        	break;
	        	case 2:fuel_x=40;
	        	break;
	        	case 3:fuel_x=-20;
	        	break;
			}
    }
}
