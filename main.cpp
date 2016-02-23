#include <stdio.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>

float fac,a,tstep=500;
int t=0;
float bp[3]={5,5,5};

void RenderScene(void) //ø�Ϩ�� 
{
     int i;
     float x,y,z,L=45.0;
     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
     glMatrixMode(GL_MODELVIEW);
     glLoadIdentity();

     gluLookAt(100,40,120,   5.0, 12.0, 5.0,    0.0f, 1.0f,0.0f);
//            �[���m        ;�������ߦ�m     ; �����y�Шt�Ϋ��V�����W�誺�V�q  
     glColor3ub(0,0,0);
     glLineWidth(3.0f);                   
     glBegin(GL_LINE_STRIP);
         glVertex3f(0.0, 0.0,0.0);glVertex3f(0.0 , L, 0.0);glVertex3f(L , L, 0);glVertex3f(L, 0, 0);glVertex3f(0.0, 0.0,0.0);
         glVertex3f(0.0, 0.0,L);glVertex3f(0.0 , L, L);glVertex3f(L , L, L); glVertex3f(L, 0, L);glVertex3f(0.0, 0.0,L);
     glEnd();    
     glBegin(GL_LINE_STRIP);glVertex3f(0.0 , L, 0.0); glVertex3f(0.0 , L, L);glEnd();
     glBegin(GL_LINE_STRIP);glVertex3f(L , L, 0);glVertex3f(L , L, L);glEnd();
     glBegin(GL_LINE_STRIP);glVertex3f(L, 0, 0);glVertex3f(L, 0, L);glEnd();

     glTranslatef(bp[0], bp[1] , bp[2]);               
     glPushMatrix();     
       
     glColor3ub(255, 0, 0);           // ����, Brownian particle
     glutSolidSphere(a, 15, 15);      // �b�|, resolution 

     glutSwapBuffers();     
}
double drand(){ //���� 0~1�����ü� 
       float r;
       r=(rand()%10000*0.0001);
       return r;
}
void TimerFunction(int value){
     FILE *fp1;
     float r,gauss;
     int i;
     fp1=fopen("trajectory.txt","a");
     t++;
     for(i=0;i<3;i++){
       do{
           r=2.0*(drand()-0.5);
           gauss=exp(-0.5* pow((r-0.0)/1.0,2.0));
       }while(drand()>gauss);
       bp[i] += fac*r; 
     }

	 fprintf(fp1,"%10.5f%10.5f%10.5f\n",bp[0],bp[1],bp[2]);
     fclose(fp1);
     glutPostRedisplay(); //�q��GLUT��s�ثe������ 
     if(t<tstep) glutTimerFunc(33,TimerFunction, 1);//���s�I�s glutTimerFunc���Ͱj�骺�@�� 

}
void SetupRC(void) //�]�w���� 
{
    GLfloat ambientLight[]={0.5f,0.5f,0.5f,1.0f};
	GLfloat diffuseLight[]={0.5f,0.5f,0.5f,1.0f};
	GLfloat specular[]={1.0f,1.0f,1.0f,1.0f};
	GLfloat specref[]={1.0f,1.0f,1.0f,1.0f};        // 
	GLfloat lightPos[]={0.0,0.0,0.0,1.0f};          //������m 

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
    glClearColor(1.0f, 1.0f, 1.0f, 0.5f);//�]�w�����C��(R,G,B,1=�������z��)      
}

void ChangeSize(GLsizei w, GLsizei h)// ��������ܤj�p�ɩI�s�����
{    
     float Range=150.0;
     if(h == 0) h = 1;   //  �קKh=0,aspectRatio-->�L���j 
     GLfloat aspectRatio; // �]�w�������e�� 
     aspectRatio = (GLfloat)w / (GLfloat)h;
     
     glViewport(0, 0, w, h);       // �NClipping volume�������ӵ��� 
     glMatrixMode(GL_PROJECTION); //�Ψөw�q clipping volume 
     glLoadIdentity();            //�N project �x�}�]�����x�} 
     
     gluPerspective(40.0f, aspectRatio, 10, -80);//�z����g 
               //   ����              ,���,���� 
     glMatrixMode(GL_MODELVIEW);//���w�ثe���ާ@�x�}�O modelview�x�} 
     glLoadIdentity();          //�N modelview�x�}�]�����x�} 
}

int main()
{
    //float a=5.0,mu=1.0;// radius of brownian particle
    //float kbT=300,dt=0.1,eta=6*M_PI*mu*a;  
    float mu,kbT,dt,eta,x;
    FILE *fp,*fp1;

    fp=fopen("index.txt","r");

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA |GLUT_DEPTH);//�إߵ����ɭn�ϥΪ��Ҧ�;��@�w�İ�,RGB��m�Ҧ� 
    glutInitWindowSize(600,600);
    glutCreateWindow("GLRect");                 //�إߵ����N���D�]�� GLRect 
    fscanf(fp,"%f%f%f%f",&a,&mu,&kbT,&dt);
    eta=6*M_PI*mu*a; 
    fac=sqrt(2*kbT*dt/eta);
    glutReshapeFunc(ChangeSize);                //�I�s ChangeSize function 
    glutDisplayFunc(RenderScene);               //��� RenderScene�o��ø�Ϩ�� 
                                                //  ;ø�Ϩ�Ʒ|�b�����Ĥ@�����,�Χ��ܵ����j�p�ɭ��s�I�s     
    glutTimerFunc(33,TimerFunction, 1);
    SetupRC();                                  //����ø�ϫe��l�� 
    glutMainLoop();

    fclose(fp);
   
    return 0;
}
