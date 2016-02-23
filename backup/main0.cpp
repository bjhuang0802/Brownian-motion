#include <stdio.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>

float fac,a;
float bp[3]={0,0,0};

void RenderScene(void) //繪圖函數 
{
     int i;
     float x,y,z;
     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
     glMatrixMode(GL_MODELVIEW);
     glLoadIdentity();

     gluLookAt(150,-45,150,   0.0, 0.0, 0.0,    0.0f, 1.0f,0.0f);
//            觀察位置        ;場景中心位置     ; 場景座標系統指向視窗上方的向量  
     glColor3ub(0,0,0);
     glBegin(GL_POINTS);
     for(i=0;i<1000;i++){
         x = i*0.1; y=x; z=x;  
         glVertex3f(x   , 0.0f, 0.0f);
         glVertex3f(0.0f, y   , 0.0f);
         glVertex3f(0.0f, 0.0f  , z);
     }
     glEnd();    
     glTranslatef(bp[0], bp[1] , bp[2]);               
     glPushMatrix();     
       
     glColor3ub(255, 0, 0);           // 紅色, Brownian particle
     glutSolidSphere(a, 15, 15);      // 半徑, resolution 

     glutSwapBuffers();     
}
void TimerFunction(int value){
    
     float r1,r2,r3;
     r1=2.0*(rand()%10000*0.0001) - 1.0;
     r2=2.0*(rand()%10000*0.0001) - 1.0;
     r3=2.0*(rand()%10000*0.0001) - 1.0;

     bp[0] += fac* r1;
	 bp[1] += fac* r2;
	 bp[2] += fac* r3;
    
     glutPostRedisplay(); //通知GLUT更新目前的視窗 
     glutTimerFunc(33,TimerFunction, 1);//重新呼叫 glutTimerFunc產生迴圈的作用 
    
}
void SetupRC(void) //設定視窗 
{
    GLfloat ambientLight[]={0.5f,0.5f,0.5f,1.0f};
	GLfloat diffuseLight[]={0.5f,0.5f,0.5f,1.0f};
	GLfloat specular[]={1.0f,1.0f,1.0f,1.0f};
	GLfloat specref[]={1.0f,1.0f,1.0f,1.0f};        // 
	GLfloat lightPos[]={0.0,0.0,0.0,1.0f};          //光的位置 

	glEnable(GL_DEPTH_TEST);

	//glEnable(GL_CCW);
	glEnable(GL_CULL_FACE);
	glEnable(GL_LIGHTING);



//	glLightModelfv(GL_LIGHT_MODEL_AMBIENT,ambientLight);
//	glLightfv(GL_LIGHT0,GL_AMBIENT,ambientLight);
	glLightfv(GL_LIGHT0,GL_SPECULAR,specular);
	glLightfv(GL_LIGHT0,GL_POSITION,lightPos);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);


//	glColorMaterial(GL_FRONT,GL_AMBIENT_AND_DIFFUSE);
	glMaterialfv(GL_FRONT,GL_SPECULAR,specref);
	glMateriali(GL_FRONT,GL_SHININESS,5);
    glClearColor(1.0f, 1.0f, 1.0f, 0.5f);//設定視窗顏色(R,G,B,1=完全不透明)      
}

void ChangeSize(GLsizei w, GLsizei h)// 當視窗改變大小時呼叫此函數
{    
     float Range=150.0;
     if(h == 0) h = 1;   //  避免h=0,aspectRatio-->無限大 
     GLfloat aspectRatio; // 設定視窗長寬比 
     aspectRatio = (GLfloat)w / (GLfloat)h;
     
     glViewport(0, 0, w, h);       // 將Clipping volume對應到整個視窗 
     glMatrixMode(GL_PROJECTION); //用來定義 clipping volume 
     glLoadIdentity();            //將 project 矩陣設為單位矩陣 
     
     gluPerspective(40.0f, aspectRatio, 10, -80);//透視投射 
               //   視角              ,近端,遠端 
     glMatrixMode(GL_MODELVIEW);//指定目前的操作矩陣是 modelview矩陣 
     glLoadIdentity();          //將 modelview矩陣設為單位矩陣 
}

int main()
{
    //float a=5.0,mu=1.0;// radius of brownian particle
    //float kbT=300,dt=0.1,eta=6*M_PI*mu*a;  
    float mu,kbT,dt,eta,x;
    FILE *fp;
    fp=fopen("index.txt","r");
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA |GLUT_DEPTH);//建立視窗時要使用的模式;單一緩衝區,RGB色彩模式 
    glutInitWindowSize(600,600);
    glutCreateWindow("GLRect");                 //建立視窗將標題設為 GLRect 
    fscanf(fp,"%f%f%f%f",&a,&mu,&kbT,&dt);
    eta=6*M_PI*mu*a; 
    fac=sqrt(2*kbT*dt/eta);
    glutReshapeFunc(ChangeSize);                //呼叫 ChangeSize function 
    glutDisplayFunc(RenderScene);               //顯示 RenderScene這個繪圖函數 
                                                //  ;繪圖函數會在視窗第一次顯示,或改變視窗大小時重新呼叫     
    glutTimerFunc(33,TimerFunction, 1);
    SetupRC();                                  //執行繪圖前初始化 
    glutMainLoop();
    return 0;
}
