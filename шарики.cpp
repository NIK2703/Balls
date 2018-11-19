#include <windows.h>
#include <stdio.h>
#include <glut.h>
#include <math.h>


#define ANGLE_RADIAN 0.01745
#define BALLS_NUMBER 10
#define BALLS_RADIUS 10
#define IMPACT_ENERGY_LOSS_COEFFICIENT 0
#define NEGLECTED_NUMBER 1
#define GRAVITY_ACCELERATION 9.81
#define WALLS_NUMBER 6
#define WALL_WIDTH 10

void myInit(void);
void myDisplay(void);			
void changeWindowSize(int,int);
void setGravityAcceleration();


float x[BALLS_NUMBER],y[BALLS_NUMBER]; // координаты шарика
float ax[BALLS_NUMBER],ay[BALLS_NUMBER]; //радиус-вектор ускорения
float vx[BALLS_NUMBER],vy[BALLS_NUMBER]; //радиус-вектор скорости

int wallRightX=800;
int wallLeftX=0;
int wallUpY=600;
int wallDownY=0;

boolean noChangeParams[BALLS_NUMBER];

int wallCoords[WALLS_NUMBER][4]={
	{wallLeftX+1,0,wallLeftX+1,wallUpY},
	{wallRightX,0,wallRightX,wallUpY},
	{200,wallDownY+1,wallRightX,wallDownY+1},
	{0,wallUpY,wallRightX,wallUpY},


};






void specialControl(int key, int x, int y)
{
	


	

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
	
	wallRightX=x;
	wallUpY=y;
	gluOrtho2D(0,wallRightX,0,wallUpY);
  
	glMatrixMode(GL_MODELVIEW);


}

void myInit(void){
	//инициализация графики
	glClearColor(1,1,1,0);
	glColor3f(0.5,1,0.5);
	glLineWidth(3);
	glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0,800,0,600);
	glMatrixMode(GL_MODELVIEW);
	for(int i=0;i<BALLS_NUMBER;i++){
	
			//начальные условия
	x[i]=rand()%wallRightX;
	y[i]=rand()%(wallUpY/2)+wallUpY/2;

	
	vx[i]=0.005*(rand()%50-100);
	vy[i]=0.005*(rand()%50-100);

	ax[i]=0;
	ay[i]=-GRAVITY_ACCELERATION*0.02;
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
	*vx2coord=-(2*vx01*m1+(m2-m1)*vx02)/(m1+m2);
	*vy2coord=-(2*vy01*m1+(m2-m1)*vy02)/(m1+m2);
	
	return (2*vx02*m2+(m1-m2)*vx01)/(m1+m2);
}
float absMassWallImpactSpeed(float xwall,float ywall,float xobj,float yobj,float vx0,float vy0,float ax,float ay,float* vy){
	if (ywall==0) {
		*vy=vy0;
		if (xobj-xwall>0) return ((vx0>0)?vx0:-((1-IMPACT_ENERGY_LOSS_COEFFICIENT)*vx0));
		else return ((vx0<0)?vx0:-((1-IMPACT_ENERGY_LOSS_COEFFICIENT)*vx0));
	}
	else if (xwall==0) {
		if ((yobj-ywall>0)) *vy=((vy0>0)?vy0:-((1-IMPACT_ENERGY_LOSS_COEFFICIENT)*vy0));
		else *vy=((vy0<0)?vy0:-((1-IMPACT_ENERGY_LOSS_COEFFICIENT)*vy0));
		return vx0;
	}
	

}


boolean checkWalls(int i){
	/*for (int j=0;j<WALLS_NUMBER;j++) {
		if (wallCoords[j][1]==wallCoords[j][3]&&(y[i]-wallCoords[j][1]<=(BALLS_RADIUS+WALL_WIDTH)/2))
			{vx[i]=absMassWallImpactSpeed(wallCoords[j][0],0,x[i],y[i],vx[i],vy[i],ax[i],ay[i],&vy[i]); return 1;} 

	}

	return 0;*/
	for (int j=0;j<WALLS_NUMBER;j++) {
		if (wallCoords[j][0]==wallCoords[j][2]) 
			if(fabs(x[i]-wallCoords[j][0])<=BALLS_RADIUS+WALL_WIDTH/2)
				if((y[i]-wallCoords[j][1]>=BALLS_RADIUS+WALL_WIDTH/2)&&(y[i]-wallCoords[j][3]<=BALLS_RADIUS+WALL_WIDTH/2))
			{vx[i]=absMassWallImpactSpeed(wallCoords[j][0],0,x[i],y[i],vx[i],vy[i],ax[i],ay[i],&vy[i]); return 1;} else; else;

		else if (wallCoords[j][1]==wallCoords[j][3]) 
				if(fabs(y[i]-wallCoords[j][1])<=BALLS_RADIUS+WALL_WIDTH/2)
					if((x[i]-wallCoords[j][0]>=BALLS_RADIUS+WALL_WIDTH/2)&&(x[i]-wallCoords[j][2]<=BALLS_RADIUS+WALL_WIDTH/2))
			{vx[i]=absMassWallImpactSpeed(0,wallCoords[j][1],x[i],y[i],vx[i],vy[i],ax[i],ay[i],&vy[i]); return 1;}
	}

	return 0;
	
	
}
float r(float x1,float x2,float y1, float y2){
	return pow((double)pow(x2-x1,2)+pow(y2-y1,2),0.5);
}




void circle(float x, float y,float r, boolean fill,double steep){
	double angle= 0;
	
	if (fill)
		glBegin(GL_TRIANGLE_FAN); else glBegin(GL_LINE_LOOP);
		for(angle=0;angle<=360;angle+=steep){
			glVertex2d(x+r*cos(angle*ANGLE_RADIAN),y+r*sin(angle*ANGLE_RADIAN));
		}
		glEnd();}
void drawChar(float x,float y,char c,double s){
	switch (c){
	case 'А': glBegin(GL_LINES);
		glVertex2d(x,y);
		glVertex2d(x+1*s,y+4*s);

		glVertex2d(x+1*s,y+4*s);
		glVertex2d(x+2*s,y);

		glVertex2d(x+0.5*s,y+2*s);
		glVertex2d(x+1.5*s,y+2*s);
		glEnd(); break;

	case 'Б':
		glBegin(GL_LINE_STRIP);
		glVertex2d(x,y+2*s);
		glVertex2d(x+2*s,y+1*s);
		glVertex2d(x,y);
		glVertex2d(x,y+4*s);
		glVertex2d(x+2*s,y+4*s);
		glEnd(); break;

	case 'В': glBegin(GL_LINE_STRIP);
		glVertex2d(x,y+2*s);
		glVertex2d(x+2*s,y+1*s);
		glVertex2d(x,y);
		glVertex2d(x,y+4*s);
		glVertex2d(x+2*s,y+3*s);
		glVertex2d(x,y+2*s);
		glEnd(); break;

	case 'Г': glBegin(GL_LINE_STRIP);
		glVertex2d(x,y);
		glVertex2d(x,y+4*s);
		glVertex2d(x+2*s,y+4*s);
		glEnd(); break;

	case 'Д': glBegin(GL_LINE_STRIP);
		glVertex2d(x,y);
		glVertex2d(x,y+1*s);
		glVertex2d(x+2*s,y+1*s);
		glVertex2d(x+2*s,y);
		glEnd();
		glBegin(GL_LINE_STRIP);
		glVertex2d(x+0.5*s,y+1*s);
		glVertex2d(x+1*s,y+4*s);
		glVertex2d(x+1.5*s,y+1*s);
		glEnd();
		break;

	case 'Е': glBegin(GL_LINE_STRIP);
		glVertex2d(x,y);
		glVertex2d(x,y+4*s);
		glVertex2d(x+2*s,y+4*s);
		glEnd(); 
		glBegin(GL_LINES);
		glVertex2d(x,y+2*s);
		glVertex2d(x+2*s,y+2*s);
		
		glVertex2d(x,y);
		glVertex2d(x+2*s,y);
		glEnd();
		break;
	case 'Ё': glBegin(GL_LINE_STRIP);
		glVertex2d(x,y);
		glVertex2d(x,y+4*s);
		glVertex2d(x+2*s,y+4*s);
		glEnd(); 
		glBegin(GL_LINES);
		glVertex2d(x,y+2*s);
		glVertex2d(x+2*s,y+2*s);
		
		glVertex2d(x,y);
		glVertex2d(x+2*s,y);
		glEnd();

		glBegin(GL_POINTS);
		glVertex2d(x+0.5*s,y+4.5*s);
		glVertex2d(x+1.5*s,y+4.5*s);
		glEnd();
		break;
	case 'Ж': 
		glBegin(GL_LINES);
		glVertex2d(x,y);
		glVertex2d(x+2*s,y+4*s);
		
		glVertex2d(x,y+4*s);
		glVertex2d(x+2*s,y);

		glVertex2d(x+1*s,y+4*s);
		glVertex2d(x+1*s,y);
		glEnd();

		break;

	case 'З': 
		glBegin(GL_LINE_STRIP);
		glVertex2d(x,y+4*s);
		glVertex2d(x+2*s,y+3*s);
		glVertex2d(x,y+2*s);
		glVertex2d(x+2*s,y+1*s);
		glVertex2d(x,y);
		glEnd();

		break;

	case 'И': 
		glBegin(GL_LINE_STRIP);
		glVertex2d(x,y+4*s);
		glVertex2d(x,y);
		glVertex2d(x+2*s,y+4*s);
		glVertex2d(x+2*s,y);
		glEnd();
		break;

	case 'Й': 
		glBegin(GL_LINE_STRIP);
		glVertex2d(x,y+4*s);
		glVertex2d(x,y);
		glVertex2d(x+2*s,y+4*s);
		glVertex2d(x+2*s,y);
		glEnd();

		glBegin(GL_LINES);
		glVertex2d(x+0.5*s,y+4.5*s);
		glVertex2d(x+1.5*s,y+4.5*s);
		glEnd();
		break;

	case 'К': 
		glBegin(GL_LINES);
		glVertex2d(x,y+4*s);
		glVertex2d(x,y);
		glVertex2d(x,y+2*s);
		glVertex2d(x+2*s,y+4*s);
		glVertex2d(x,y+2*s);
		glVertex2d(x+2*s,y);
		glEnd();
		break;
	
	case 'Л': 
		glBegin(GL_LINE_STRIP);
		glVertex2d(x,y);
		glVertex2d(x+1*s,y+4*s);
		glVertex2d(x+2*s,y);
		glEnd();
		break;

	case 'М': 
		glBegin(GL_LINE_STRIP);
		glVertex2d(x,y);
		glVertex2d(x,y+4*s);
		glVertex2d(x+1*s,y);
		glVertex2d(x+2*s,y+4*s);
		glVertex2d(x+2*s,y);
		glEnd();
		break;
	case 'Н': glBegin(GL_LINES);
		glVertex2d(x,y);
		glVertex2d(x,y+4*s);
		glVertex2d(x,y+2*s);
		glVertex2d(x+2*s,y+2*s);
		glVertex2d(x+2*s,y);
		glVertex2d(x+2*s,y+4*s);
		glEnd();
		break;

	case 'О': glBegin(GL_LINE_LOOP);
		glVertex2d(x+1*s,y);
		glVertex2d(x+2*s,y+2*s);
		glVertex2d(x+1*s,y+4*s);
		glVertex2d(x,y+2*s);
		glEnd();
		break;

	case 'П': glBegin(GL_LINE_STRIP);
		glVertex2d(x,y);
		glVertex2d(x,y+4*s);
		glVertex2d(x+2*s,y+4*s);
		glVertex2d(x+2*s,y);
		glEnd(); break;

	case 'Р': glBegin(GL_LINE_STRIP);
		glVertex2d(x,y);
		glVertex2d(x,y+4*s);
		glVertex2d(x+2*s,y+3*s);
		glVertex2d(x,y+2*s);
		glEnd(); break;

	case 'С': glBegin(GL_LINE_STRIP);
		glVertex2d(x+2*s,y+4*s);
		glVertex2d(x,y+2*s);
		glVertex2d(x+2*s,y);
		glEnd(); break;

	case 'Т': glBegin(GL_LINES);
		glVertex2d(x+1*s,y);
		glVertex2d(x+1*s,y+4*s);
		glVertex2d(x,y+4*s);
		glVertex2d(x+2*s,y+4*s);
		glEnd();
		break;

	case 'У': glBegin(GL_LINES);
		glVertex2d(x,y);
		glVertex2d(x+2*s,y+4*s);
		glVertex2d(x,y+4*s);
		glVertex2d(x+1*s,y+2*s);
		
		glEnd();
		break;

	case 'Ф': glBegin(GL_LINES);
		glVertex2d(x+1*s,y);
		glVertex2d(x+1*s,y+4*s);
		glEnd();
		glBegin(GL_LINE_LOOP);
		glVertex2d(x,y+2*s);
		glVertex2d(x+1*s,y+3.25*s);
		glVertex2d(x+2*s,y+2*s);
		glVertex2d(x+1*s,y+0.75*s);
	    glEnd();
		break;

	case 'Х':glBegin(GL_LINES);
		glVertex2d(x,y);
		glVertex2d(x+2*s,y+4*s);
		glVertex2d(x,y+4*s);
		glVertex2d(x+2*s,y);
		glEnd();
		break;

	case 'Ц':
		glBegin(GL_LINE_STRIP);
		glVertex2d(x,y+4*s);
		glVertex2d(x,y);
		glVertex2d(x+2*s,y);
		glVertex2d(x+2*s,y+4*s);
		glEnd();
		glBegin(GL_LINE_STRIP);
		glVertex2d(x+2*s,y);
		glVertex2d(x+2.5*s,y);
		glVertex2d(x+2.5*s,y-0.75*s);
		glEnd();
		break;

	case 'Ч':
		glBegin(GL_LINE_STRIP);
		glVertex2d(x,y+4*s);
		glVertex2d(x,y+2*s);
		glVertex2d(x+2*s,y+2*s);
		glVertex2d(x+2*s,y+4*s);
		glVertex2d(x+2*s,y);
		glEnd();
		break;

	case 'Ш':
		glBegin(GL_LINE_STRIP);
		glVertex2d(x,y+4*s);
		glVertex2d(x,y);
		glVertex2d(x+1*s,y);
		glVertex2d(x+1*s,y+4*s);
		glEnd();
		glBegin(GL_LINE_STRIP);
		glVertex2d(x+1*s,y);
		glVertex2d(x+2*s,y);
		glVertex2d(x+2*s,y+4*s);
		glEnd();
		break;

	case 'Щ':
		glBegin(GL_LINE_STRIP);
		glVertex2d(x,y+4*s);
		glVertex2d(x,y);
		glVertex2d(x+1*s,y);
		glVertex2d(x+1*s,y+4*s);
		glEnd();
		glBegin(GL_LINE_STRIP);
		glVertex2d(x+1*s,y);
		glVertex2d(x+2*s,y);
		glVertex2d(x+2*s,y+4*s);
		glEnd();
		glBegin(GL_LINE_STRIP);
		glVertex2d(x+2*s,y);
		glVertex2d(x+2.5*s,y);
		glVertex2d(x+2.5*s,y-0.75*s);
		glEnd();
		break;

	case 'Ъ':
		glBegin(GL_LINE_STRIP);
		glVertex2d(x,y+2*s);
		glVertex2d(x+2*s,y+1*s);
		glVertex2d(x,y);
		glVertex2d(x,y+4*s);
		glVertex2d(x-0.75*s,y+4*s);
		glEnd(); break;

	case 'Ы':
		glBegin(GL_LINE_STRIP);
		glVertex2d(x,y+2*s);
		glVertex2d(x+1.75*s,y+1*s);
		glVertex2d(x,y);
		glVertex2d(x,y+4*s);
		glEnd();
		glBegin(GL_LINES);
		glVertex2d(x+2*s,y);
		glVertex2d(x+2*s,y+4*s);
		glEnd();
		break;

	case 'Ь':
		glBegin(GL_LINE_STRIP);
		glVertex2d(x,y+2*s);
		glVertex2d(x+2*s,y+1*s);
		glVertex2d(x,y);
		glVertex2d(x,y+4*s);
		glEnd(); break;

	case 'Э':
		glBegin(GL_LINE_STRIP);
		glVertex2d(x,y+4*s);
		glVertex2d(x+2*s,y+2*s);
		glVertex2d(x,y);
		glEnd();
		glBegin(GL_LINES);
		glVertex2d(x+2*s,y+2*s);
		glVertex2d(x,y+2*s);
		glEnd();
		break;

	case 'Ю':
		glBegin(GL_LINES);
		glVertex2d(x,y);
		glVertex2d(x,y+4*s);
		glVertex2d(x,y+2*s);
		glVertex2d(x+0.5*s,y+2*s);
		glEnd();
		glBegin(GL_LINE_LOOP);
		glVertex2d(x+0.5*s,y+2*s);
		glVertex2d(x+1.25*s,y+4*s);
		glVertex2d(x+2*s,y+2*s);
		glVertex2d(x+1.25*s,y);
		glEnd();
		break;

   case 'Я':
		glBegin(GL_LINE_STRIP);
		glVertex2d(x+2*s,y);
		glVertex2d(x+2*s,y+4*s);
		glVertex2d(x,y+3*s);
		glVertex2d(x+2*s,y+2*s);
		glVertex2d(x,y);
		glEnd();

		break;

	case '(':
		glBegin(GL_LINE_STRIP);
		glVertex2d(x+2*s,y+4*s);
		glVertex2d(x+1*s,y+2*s);
		glVertex2d(x+2*s,y);
		glEnd(); break;
	case ')':
		glBegin(GL_LINE_STRIP);
		glVertex2d(x,y+4*s);
		glVertex2d(x+1*s,y+2*s);
		glVertex2d(x,y);
		glEnd(); break;
	case ',':
		glBegin(GL_LINES);
		glVertex2d(x+0.75*s,y);
		glVertex2d(x,y-1.25*s);
		glVertex2d(x,y);
		glEnd(); break;
	case '.':
		glBegin(GL_POINTS);
		glVertex2d(x,y);
		glEnd(); break;
	case '-':
		glBegin(GL_LINES);
		glVertex2d(x,y+2*s);
		glVertex2d(x+2*s,y+2*s);
		glEnd(); break;
	case '1':
		glBegin(GL_LINE_STRIP);
		glVertex2d(x,y+2*s);
		glVertex2d(x+2*s,y+4*s);
		glVertex2d(x+2*s,y);
		glEnd(); break;
	case '2':
		glBegin(GL_LINE_STRIP);
		glVertex2d(x,y+4*s);
		glVertex2d(x+2*s,y+2.5*s);
		glVertex2d(x,y+1*s);
		glVertex2d(x,y);
		glVertex2d(x+2*s,y);
		glEnd(); break;
	case '3':
		glBegin(GL_LINE_STRIP);
		glVertex2d(x,y+4*s);
		glVertex2d(x+2*s,y+3*s);
		glVertex2d(x,y+2*s);
		glVertex2d(x+2*s,y+1*s);
		glVertex2d(x,y);
		glEnd();

		break;
	case '4':
		glBegin(GL_LINE_STRIP);
		glVertex2d(x+2*s,y);
		glVertex2d(x+2*s,y+4*s);
		glVertex2d(x,y+2*s);
		glVertex2d(x+2.5*s,y+2*s);
		glEnd();
		break;
	case '5':
		glBegin(GL_LINE_STRIP);
		glVertex2d(x+2*s,y+4*s);
		glVertex2d(x,y+4*s);
		glVertex2d(x,y+3*s);
		glVertex2d(x+2*s,y+1.5*s);
		glVertex2d(x,y);
		
		glEnd(); break;

	case '6':
		glBegin(GL_LINE_STRIP);
		glVertex2d(x+1*s,y+4*s);
		glVertex2d(x,y+2*s);
		glVertex2d(x+1*s,y);
		glVertex2d(x+2*s,y+2*s);
		glVertex2d(x,y+2*s);
		
		glEnd(); break;

	case '7':
		glBegin(GL_LINE_STRIP);
		glVertex2d(x,y+4*s);
		glVertex2d(x+2*s,y+4*s);
		glVertex2d(x,y);
		glEnd();
		glBegin(GL_LINES);
		glVertex2d(x+0.25*s,y+2*s);
		glVertex2d(x+1.75*s,y+2*s);
		glEnd();
		break;

	case '8':
		glBegin(GL_LINE_LOOP);
		glVertex2d(x+1*s,y+4*s);
		glVertex2d(x,y+3*s);
	    glVertex2d(x+2*s,y+1*s);
		glVertex2d(x+1*s,y);
		glVertex2d(x,y+1*s);
		glVertex2d(x+2*s,y+3*s);
		glEnd(); break;

	case '9':
		glBegin(GL_LINE_STRIP);
		glVertex2d(x+1*s,y);
		glVertex2d(x+2*s,y+2*s);
		glVertex2d(x+1*s,y+4*s);
		glVertex2d(x,y+2*s);
		glVertex2d(x+2*s,y+2*s);
		
		glEnd(); break;

	case '0':
		glBegin(GL_LINE_LOOP);
		glVertex2d(x+1*s,y);
		glVertex2d(x+2*s,y+2*s);
		glVertex2d(x+1*s,y+4*s);
		glVertex2d(x,y+2*s);
		glEnd();

		glBegin(GL_LINES);
		glVertex2d(x+1.5*s,y+3*s);
		glVertex2d(x+0.5*s,y+1*s);
		glEnd();

		break;

	case ':':
		glBegin(GL_POINTS);
		glVertex2d(x,y+0.75*s);
		glVertex2d(x,y+3.25*s);
		glEnd();
		break;
	case '!':
		glPointSize(5);
		glBegin(GL_POINTS);
		glVertex2d(x+0.5,y);
		glEnd();
		glPointSize(3);

		glBegin(GL_LINES);
		glVertex2d(x+0.5,y+0.75*s);
		glVertex2d(x+0.5,y+4*s);
		glEnd();
		break;
	}
}
void write(float x, float y,char* string,float scale){


	int l=strlen(string);

	int m=0,ns=0;

	for(int i=0;i<l;i++){
		if(string[i]=='\n') {m=-1;ns++;}
		drawChar(x+3*m*scale,y-5*ns*scale,string[i],scale);
		m++;} 


}
void aligWriteCenter(float y,char* string,float scale,float xcenter){



		float xs;
		int length=strlen(string);
		xs=xcenter-(3*length*scale)/2;
		write(xs,y,string,scale);

}

void drawWall(int i){
	glLineWidth(WALL_WIDTH);
	glColor3f(0.5,0.5,0.5);
	glBegin(GL_LINES);
	if(wallCoords[i][1]==0){
	glVertex2d(wallCoords[i][0],0);
	glVertex2d(wallCoords[i][0],wallUpY);
	}

	if(wallCoords[i][0]==0){
	glVertex2d(0,wallCoords[i][1]);
	glVertex2d(wallRightX,wallCoords[i][1]);
	}
	glEnd();





	glColor3f(0.5,1,0.5);
}
void myDisplay(void){
	glClear(GL_COLOR_BUFFER_BIT);
		
	for(int i=0;i<WALLS_NUMBER; i++)
		drawWall(i);


	for(int i=0;i<BALLS_NUMBER; i++){
		noChangeParams[i]=0;
	for(int j=0;j<BALLS_NUMBER;j++)
			if (i!=j)
				if (r(x[i],x[j],y[i],y[j])<=BALLS_RADIUS*2&&r(x[i],x[j],y[i],y[j])>=BALLS_RADIUS/2){
					vx[i]=absElasticImpactSpeed(vx[i],vy[j],vx[i],vy[j],2,2,&vy[i],&vx[j],&vy[j],i,j);
					noChangeParams[i]=noChangeParams[j]=1;
				}
				
	if (!checkWalls(i)&&!noChangeParams[i]){
	vx[i]+=ax[i];
	vy[i]+=ay[i];}
	
	x[i]+=vx[i];
	y[i]+=vy[i];
	
		
	circle(x[i],y[i],BALLS_RADIUS,1,10);
	
	
	}
	

	

	
	Sleep(10);
	glutPostRedisplay();
	glFlush();
	
}



