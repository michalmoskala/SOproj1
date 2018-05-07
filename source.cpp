#include<iostream>
#include<ncurses.h>
#include<unistd.h>
#include<thread>
#include<cstdlib>
#include<ctime>

#define WINWIDTH 100
#define WINHEIGHT 20

int mvsd=0;
int mvdn=0;
int speed=5;
bool dead=false;
int carX;
int carY;


class Obstacle{
private:
	int x;
	int y;
public:
	int getX(){return x;}
	int getY(){return y;}
	void resetX(){x=50;}

	Obstacle(int a, int b){x=a;y=b;}

	void draw(){mvprintw(y,x,"O");}
	void update(){x--;}
	void clear(){mvprintw(y,x," ");}
	bool timeToDie(){return (x<2);}
	bool reCreate(int a, int b){x=a;y=b;}

};

Obstacle obs(50,3);

class Car{
private:
	int x;
	int y;
	int wait;

public:
	int getX(){return x;}
	int getY(){return y;}
	int getWait(){return wait;}

	Car(int a,int b,int e){x=a;y=b;wait=e;}

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


Car car(2,3,20000);

class Decoration{
private:
	int x;
	int y;
public:
	int getX(){return x;}
	int getY(){return y;}
	
	Decoration(int a, int b){x=a;y=b;}
	
	void draw(){	mvprintw(y+0,x,"| || | _");
		    	mvprintw(y+1,x,"| || || |");
			mvprintw(y+2,x," \_  || |");
		    	mvprintw(y+3,x,"   |  _/");
		    	mvprintw(y+4,x,"   | |");
			mvprintw(y+5,x,"   |_|");
		   }
	
	void clear(){
			mvprintw(y+0,x,"         ");
			mvprintw(y+1,x,"         ");
			mvprintw(y+2,x,"         ");
			mvprintw(y+3,x,"         ");
			mvprintw(y+4,x,"         ");
			mvprintw(y+5,x,"         ");

		}
	}
	
	void update(){x--;}
	

}

Decoration gagduz(40,70);

void drawBox(int width, int height,bool sswitch){
        for (int i=1;i<height-1;i++){
		mvprintw(i,0,"X");
		mvprintw(i,width-1,"X");

                }
        for (int i=0;i<width;i++){
		if(sswitch){
                mvprintw(0,i,"-");
                mvprintw(height-1,i,"-");
                }
		else{
		mvprintw(0,i,"=");
		mvprintw(height-1,i,"=");
		}
		sswitch=!sswitch;
	}
}




void repeat(Car car){
	while(!dead){
	carX=car.getX();
	carY=car.getY();

	car.clear();
	car.mvr(mvsd,mvdn);
	mvsd=0;
	mvdn=0;
	car.draw();
	refresh();
	usleep(car.getWait());
	}

	car.clear();

}

void readbutton(){
	while(!dead){
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
	usleep(1000);
}

}

void raiseSpeed(){
	mvprintw(WINHEIGHT,0,"SCORE:");
	while(!dead){
		speed++;
		usleep(1000000);
	mvprintw(WINHEIGHT,speed,"-");
}
}

void obsMovement(Obstacle obs){

	std::srand(std::time(nullptr));
	int r=std::rand()%8;
	obs.reCreate(50,r+2);

	while(!dead){
	obs.clear();
	obs.update();
	obs.draw();

	if((obs.getX()<=carX+6&&obs.getX()>=carX&&obs.getY()==carY)
	||(obs.getX()<=carX+7&&obs.getX()>=carX&&obs.getY()==carY+1)) dead=true;

	usleep(1000000/speed);
	if(obs.timeToDie())
		{
		obs.clear();
		int r=std::rand()%5;
		r+=7;
		usleep(10000000/r);
		r=std::rand()%8;
		int rr=std::rand()%20;
		obs.reCreate(rr+40,r+2);
		}
	}
	obs.clear();
}

void decMovement(Decoration dec){

	while(!dead){
	obs.clear();
	obs.update();
	obs.draw();
		
	usleep(1000000/speed);
	
}

void frameMovement(){
	bool sswitch=true;
	while(!dead){
	sswitch=!sswitch;
	drawBox(WINWIDTH,WINHEIGHT,sswitch);

	usleep(1000000/speed);
	}

}

int main(){
	initscr();
	curs_set(0);

	std::thread t0(obsMovement,std::ref(obs));
	std::thread t0a(obsMovement,std::ref(obs));
	std::thread t0b(obsMovement,std::ref(obs));
	std::thread t0c(obsMovement,std::ref(obs));
	std::thread t0d(obsMovement,std::ref(obs));

	std::thread t1(repeat,std::ref(car));
	std::thread t2(readbutton);
	std::thread t3(frameMovement);
	std::thread t4(raiseSpeed);
	std::thread t5(decMovement,std::ref(gagduz));
	
	t0.join();
	t1.join();
	t2.join();
	t3.join();
	t4.join();
	t5.join();

	endwin();
	return 0;
}

