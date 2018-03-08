#include<ncurses.h>
#include<unistd.h>
#include<thread>

#define WINWIDTH 40
#define WINHEIGHT 20

class Ball{
private:
	int spX;
	int spY;
	int x;
	int y;
	int wait;

public:
	int bounces=0;
	int getX(){return x;}
	int getY(){return y;}
	int getBounces(){return bounces;}
	int getWait(){return wait;}

	Ball(int a,int b,int c,int d,int e){x=a;y=b;spX=c;spY=d;wait=e;}

	void update()
	{

		if (x>=WINWIDTH-2||x<=1)
		{
			spX=-spX;
			bounces++;
		}

		x+=spX;

		if (y>=WINHEIGHT-2||y<=1)
		{
			spY=-spY;
			bounces++;
		}
		y+=spY;
	}

	void draw(){mvprintw(y,x,"O");}

	void clear(){
	if (y==0||y==WINHEIGHT-1||x==0||x==WINWIDTH-1)
		mvprintw(y,x,"X");
	else
		mvprintw(y,x," ");

	}

};



void drawBox(int width, int height){
        for (int i=1;i<height-1;i++){
                mvprintw(i,0,"X");
                mvprintw(i,width-1,"X");
                }
        for (int i=0;i<width;i++){
                mvprintw(0,i,"X");
                mvprintw(height-1,i,"X");
                }
}




void repeat(Ball ball){
	while(ball.bounces<20){
	ball.clear();
	ball.update();
	ball.draw();
	refresh();
	usleep(ball.getWait());
	}

	ball.clear();

}




int main(){
	initscr();

	Ball ball(3,3,1,1,70000);
	Ball ball2(4,5,2,1,30000);
	Ball ball3(7,2,0,2,10000);

	drawBox(WINWIDTH,WINHEIGHT);

	std::thread t1(repeat,std::ref(ball));
	std::thread t2(repeat,std::ref(ball2));
	std::thread t3(repeat,std::ref(ball3));

	t1.join();
	t2.join();
	t3.join();

	endwin();
	return 0;
}

