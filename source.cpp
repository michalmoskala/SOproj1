#include<iostream>
#include<ncurses.h>
#include<unistd.h>
#include<thread>

#define WINWIDTH 40
#define WINHEIGHT 50

int mvsd=0;
int mvdn=0;


class Ball{
private:
	int x;
	int y;
	int wait;

public:
	int getX(){return x;}
	int getY(){return y;}
	int getWait(){return wait;}

	Ball(int a,int b,int e){x=a;y=b;wait=e;}

	void draw(){	mvprintw(y+0,x,".'-`-._");
			mvprintw(y+1,x,"'O--O--'");}

	void clear(){
		if (y==0||y==WINHEIGHT-1||x==0||x==WINWIDTH-1)
			mvprintw(y,x,"X");
		else{
			mvprintw(y+0,x,"       ");
			mvprintw(y+1,x,"        ");
		}
	}

	void mvr(int m,int n){x+=m; y+=n;}

};

Ball ball(2,2,50000);

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
	while(true){
	ball.clear();
	ball.mvr(mvsd,mvdn);
	mvsd=0;
	mvdn=0;
	ball.draw();
	refresh();
	usleep(ball.getWait());
	}

	ball.clear();

}

void readbutton(){
	while(true){
	char input;
	std::cin >> input;
	if (input=='a')
		mvsd=-1;
	else if (input=='d')
		mvsd=1;
	else if (input=='s')
		mvdn=1;
	else if (input=='w')
		mvdn=-1;
}

}



int main(){
	initscr();

	drawBox(WINWIDTH,WINHEIGHT);

	std::thread t1(repeat,std::ref(ball));
	std::thread t2(readbutton);

	t1.join();
	t2.join();

	endwin();
	return 0;
}

