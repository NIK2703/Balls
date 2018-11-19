#include <windows.h>
#include <conio.h>
#include <stdio.h>
#include <glut.h>
#include <math.h>
#include <time.h>


#define ANGLE_RADIAN 0.01745
#define BALLS_NUMBER 25
#define BALLS_RADIUS 7
#define IMPACT_ENERGY_LOSS_COEFFICIENT 0.005
#define GRAVITY_ACCELERATION 9.81
#define WALLS_NUMBER 12
#define WALL_WIDTH 40
#define MOVING_RESISTANCE 0.1
#define MAX_SPEED 8


void myInit(void);
void myDisplay(void);			
void changeWindowSize(int,int);
void setGravityAcceleration();


float x[BALLS_NUMBER],y[BALLS_NUMBER]; // координаты шарика
float ax[BALLS_NUMBER],ay[BALLS_NUMBER]; //радиус-вектор ускорения
float vx[BALLS_NUMBER],vy[BALLS_NUMBER]; //радиус-вектор скорости
boolean jump=0;
float jumpSpeed=15;

int wallRightX=800;
int wallLeftX=1;
int wallUpY=600;
int wallDownY=1;
int screenShiftX=0,screenShiftY=0;



boolean noChangeParams[BALLS_NUMBER];

int wallCoords[WALLS_NUMBER][4]={
	{wallLeftX,wallDownY,wallLeftX,wallUpY},
	{wallLeftX,wallDownY,wallRightX,wallDownY},
	{wallLeftX,wallUpY,wallRightX,wallUpY},
	{wallRightX,wallDownY,wallRightX,wallUpY},

	{0,200,200,200},
	{600,400,800,400},
	{300,300,500,300},
	
};






void specialControl(int key, int x, int y)
{

	/*switch(key){
	case GLUT_KEY_UP : for(int i=0;i<BALLS_NUMBER;i++) jump=1; break;
	case GLUT_KEY_LEFT : for(int i=0;i<BALLS_NUMBER;i++) vx[i]=-MAX_SPEED; break;
	case GLUT_KEY_RIGHT : for(int i=0;i<BALLS_NUMBER;i++) vx[i]=MAX_SPEED; break;

	}*/
	

}

void main(int argc, char **argv){
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
	glutInitWindowSize(wallRightX-wallLeftX,wallUpY-wallDownY);
	glutInitWindowPosition(300,50);
	glutCreateWindow("LAB 6");
	glutSpecialFunc(specialControl);
	glutReshapeFunc(changeWindowSize);
	myInit();
	glutDisplayFunc(myDisplay);
	glutMainLoop();


	
}
void changeWindowSize(int x, int y){
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
 	glViewport(0, 0, x, y);
	
	
	gluOrtho2D(0+screenShiftX,wallRightX+screenShiftX,0+screenShiftY,wallUpY+screenShiftY);
  
	glMatrixMode(GL_MODELVIEW);


}
float r(float x1,float x2,float y1, float y2);
void generateRandomLevel(){
	srand(time(0));
	for (int i=4;i<WALLS_NUMBER;i++){
		
			
		if(rand()%2){
			wallCoords[i][0]=(rand()%10+i)*50+1;
			wallCoords[i][2]=(rand()%10+rand()%5+i)*50+1;
			wallCoords[i][1]=wallCoords[i][3]=(rand()%12)*50+1;
		}
		else{
			wallCoords[i][1]=(rand()%12)*50+1;
			wallCoords[i][3]=(rand()%12)*50+1;
			wallCoords[i][0]=wallCoords[i][2]=(rand()%6+i)*50+1;
		}
		}
}
void myInit(void){
	//инициализация графики
	glClearColor(1,1,1,0);
	glColor3f(0.5,0.75,1);
	glLineWidth(3);
	glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0,800,0,600);
	glMatrixMode(GL_MODELVIEW);
	for(int i=0;i<BALLS_NUMBER;i++){
	
			//начальные условия
		/*generateRandomLevel();*/
	x[i]=rand()%wallRightX;
	y[i]=rand()%wallUpY;

	
	vx[i]=0.02*(rand()%50-100);
	vy[i]=0.02*(rand()%50-100);

	ax[i]=0;
	ay[i]=-GRAVITY_ACCELERATION/50;
	}

	

	
}




float calcObjCoord(float x0,float y0,float vx,float vy,float ax,float ay, float time,float* ycoord){
	*ycoord=y0+vy*time+ay*time*time/2;
	
	return x0+vx*time+ax*time*time/2;
}
float calcObjSpeed(float vx,float vy,float ax,float ay, float time,float* yspeed){
	*yspeed=vy+ay*time;
	return vx+ax*time;
}


float absElasticImpactSpeed(float vx01,float vy01,float vx02, float vy02,float m1, float m2,float* vy1coord,float* vx2coord,float* vy2coord,int i1, int i2){
	
	*vy1coord=(2*vy02*m2+(m1-m2)*vy01)/(m1+m2);
	*vx2coord=(2*vx01*m1+(m2-m1)*vx02)/(m1+m2);
	*vy2coord=(2*vy01*m1+(m2-m1)*vy02)/(m1+m2);
	
	return (2*vx02*m2+(m1-m2)*vx01)/(m1+m2);
}
float absMassWallImpactSpeed(int xwall,int ywall,int x2wall,int y2wall,float xobj,float yobj,float vx0,float vy0,float ax,float ay,float* vy){
	if (ywall==0) {
		*vy=vy0;
		if (xobj-xwall>0) return ((vx0>0)?vx0:-((1-IMPACT_ENERGY_LOSS_COEFFICIENT)*vx0));
		else return ((vx0<0)?vx0:-((1-IMPACT_ENERGY_LOSS_COEFFICIENT)*vx0));
	}
	if (xwall==0) {
		if ((yobj-ywall>0)) *vy=((vy0>0)?vy0:-((1-IMPACT_ENERGY_LOSS_COEFFICIENT)*vy0));
		else *vy=((vy0<0)?vy0:-((1-IMPACT_ENERGY_LOSS_COEFFICIENT)*vy0));
		return vx0;
	}

	/*else {
		float tan=(x2wall-xwall)/(y2wall-ywall);
		if(xobj<tan*yobj&&yobj>tan/xobj){
		*vy=((vy0<0)?-vy0:((1-IMPACT_ENERGY_LOSS_COEFFICIENT)*vy0)*0.5);
		return ((vx0<0)?vx0:-((1-IMPACT_ENERGY_LOSS_COEFFICIENT)*vx0)*0.5);
		}
		else
			{
		*vy=((vy0>0)?-vy0:((1-IMPACT_ENERGY_LOSS_COEFFICIENT)*vy0)*0.5);
		return ((vx0<0)?vx0:-((1-IMPACT_ENERGY_LOSS_COEFFICIENT)*vx0)*0.5);
		}
	}*/

	

}


int checkWalls(int i){
	
	for (int j=0;j<WALLS_NUMBER;j++) {
		
		
		if (abs(wallCoords[j][0]-wallCoords[j][2])<=2) 
			if(fabs(x[i]-wallCoords[j][0])<=BALLS_RADIUS+WALL_WIDTH/2)
				if((y[i]-wallCoords[j][1]>=-BALLS_RADIUS)&&(y[i]-wallCoords[j][3]<=BALLS_RADIUS))
			{vx[i]=absMassWallImpactSpeed(wallCoords[j][0],0,0,0,x[i],y[i],vx[i],vy[i],ax[i],ay[i],&vy[i]); 
		
		if (x[i]-wallCoords[j][0]>0) return 2; else return 4;} else; else;

		else if (abs(wallCoords[j][1]-wallCoords[j][3])<=2) 
				if(fabs(y[i]-wallCoords[j][1])<=BALLS_RADIUS+WALL_WIDTH/2)
					if((x[i]-wallCoords[j][0]>=-BALLS_RADIUS)&&(x[i]-wallCoords[j][2]<=BALLS_RADIUS))
			{vx[i]=absMassWallImpactSpeed(0,wallCoords[j][1],0,0,x[i],y[i],vx[i],vy[i],ax[i],ay[i],&vy[i]); 
		
		if (y[i]-wallCoords[j][1]>0) return 1;else return 3;}

					
		}

	return 0;
	
	
}
float r(float x1,float x2,float y1, float y2){
	return pow((double)(pow(x2-x1,2)+pow(y2-y1,2)),0.5);
}




void circle(float x, float y,float r, boolean fill,double steep){
	double angle= 0;
	
	if (fill)
		glBegin(GL_TRIANGLE_FAN); else glBegin(GL_LINE_LOOP);
		for(angle=0;angle<=360;angle+=steep){
			glVertex2d(x+r*cos(angle*ANGLE_RADIAN),y+r*sin(angle*ANGLE_RADIAN));
		}
		glEnd();}

void drawWall(int i){
	
	glLineWidth(WALL_WIDTH);
	glColor3f(0.7,0.7,0.7);
	glBegin(GL_LINES);
	glVertex2d(wallCoords[i][0],wallCoords[i][1]);
	glVertex2d(wallCoords[i][2],wallCoords[i][3]);
	glEnd();


	glColor3f(0.7,0.85,1);
}
void changeView(){
	screenShiftX=x[0]-wallRightX/2;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	gluOrtho2D(0+screenShiftX,wallRightX+screenShiftX,0+screenShiftY,wallUpY+screenShiftY);
	glMatrixMode(GL_MODELVIEW);
}
void myDisplay(void){
	glClear(GL_COLOR_BUFFER_BIT);
		

	for(int i=0;i<WALLS_NUMBER; i++)
		drawWall(i);


	for(int i=0;i<BALLS_NUMBER; i++){
		
	
				
	if (!checkWalls(i)){
	vx[i]+=ax[i];
	vy[i]+=ay[i];}

	if (jump==1&&checkWalls(i)==1) {
		vy[i]=jumpSpeed;
		jump=0;
	}
	
	x[i]+=vx[i];
	y[i]+=vy[i];
	

	/*if (vx[i]<0) vx[i]+=MOVING_RESISTANCE; else if (vx[i]>0) vx[i]-=MOVING_RESISTANCE;*/
	
	circle(x[i],y[i],BALLS_RADIUS,1,10);
	
	for(int j=0;j<BALLS_NUMBER;j++)
			if (i!=j)
				if (r(x[i],x[j],y[i],y[j])<=BALLS_RADIUS*2){
					vx[i]=absElasticImpactSpeed(vx[i],vy[i],vx[j],vy[j],2,2,&vy[i],&vx[j],&vy[j],i,j);
					
				}
	}
	
	/*changeView();*/
	
	Sleep(10);
	glutPostRedisplay();
	glFlush();
	
}



