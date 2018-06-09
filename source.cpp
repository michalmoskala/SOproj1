#include<iostream>
#include<ncurses.h>
#include<unistd.h>
#include<thread>
#include<cstdlib>
#include<ctime>
#include<fstream>

#define WINWIDTH 100
#define WINHEIGHT 20

int mvsd=0;
int mvdn=0;
int speed=5;
bool dead=false;
int carX;
int carY;
int hp=8;


class MovingObject{
protected:
	int x;
	int y;
public:
	int getX(){return x;}
	int getY(){return y;}
};


class ClearableObject:public MovingObject{
public:
	bool reCreate(int a, int b){x=a;y=b;}
	void update(){x--;}

};

class Collectable:public ClearableObject{
public:
	void clear(){mvprintw(y,x," ");}
	bool timeToDie(){return (x<2);}
	void killX(){x=-2;}

};

class Car:public MovingObject{
private:
	int wait;

public:
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

	void mvr(int m,int n){
		if ((m<0&&x>2)||(m>0&&x<WINWIDTH-9))
			x+=m;
		if ((n<0&&y>1)||(n>0&&y<WINHEIGHT-3))
			y+=n;
	}

};



class Obstacle:public Collectable{
public:

	Obstacle(int a, int b){x=a;y=b;}

	void draw(){mvprintw(y,x,"X");}
	

};

Obstacle obs(50,3);


class Health:public Collectable{
public:
	
	Health(int a, int b){x=a;y=b;}

	void draw(){mvprintw(y,x,"H");}
	

};

Health hea(30,7);




class Reward:public Collectable{
public:
	
	Reward(int a, int b){x=a;y=b;}

	void draw(){mvprintw(y,x,"P");}
	
};

Reward rew(40,6);


Car car(2,3,20000);



class Decoration:public ClearableObject{
public:
	Decoration(int a, int b){x=a;y=b;}

	void draw(){	mvprintw(y+0,x,"| || | _");
		    	mvprintw(y+1,x,"| || || |");
			mvprintw(y+2,x," \\_  || |");
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

	bool timeToDie(){return (x<-4);}
	

};

Decoration gagduz(70,40);

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
	if (hp<=0) dead=true;
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
	mvprintw(WINHEIGHT+1,0,"                                                 ");
	for (int i=6;i<6+speed;i++)
		mvprintw(WINHEIGHT,i,"-");
	mvprintw(WINHEIGHT+1,0,"HP:");
	for (int i=3;i<3+hp;i++)
		mvprintw(WINHEIGHT+1,i,"+");
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
	||(obs.getX()<=carX+7&&obs.getX()>=carX&&obs.getY()==carY+1))
	{
		hp--;
		obs.killX();

	}

	usleep(1000000/speed);
	if(obs.timeToDie())
		{
		obs.clear();
		int r=std::rand()%5;
		r+=7;
		usleep(10000000/r);
		r=std::rand()%(WINHEIGHT-3);
		int rr=std::rand()%20;
		obs.reCreate(rr+WINWIDTH-20,r+2);
		}
	}
	obs.clear();
}

void rewMovement(Reward rew){

	std::srand(std::time(nullptr));
	int r=std::rand()%8;
	rew.reCreate(50,r+2);

	while(!dead){
	rew.clear();
	rew.update();
	rew.draw();

	if((rew.getX()<=carX+6&&rew.getX()>=carX&&rew.getY()==carY)
	||(rew.getX()<=carX+7&&rew.getX()>=carX&&rew.getY()==carY+1)){
		speed+=3;
		rew.killX();
	}

	usleep(1000000/speed);
	if(rew.timeToDie())
		{
		rew.clear();
		int r=std::rand()%5;
		r+=7;
		usleep(10000000/r);
		r=std::rand()%(WINHEIGHT-3);
		int rr=std::rand()%20;
		rew.reCreate(rr+WINWIDTH-20,r+2);
		}
	}
	rew.clear();
}


void heaMovement(Health hea){

	std::srand(std::time(nullptr));
	int r=std::rand()%8;
	hea.reCreate(50,r+2);

	while(!dead){
	hea.clear();
	hea.update();
	hea.draw();

	if((hea.getX()<=carX+6&&hea.getX()>=carX&&hea.getY()==carY)
	||(hea.getX()<=carX+7&&hea.getX()>=carX&&hea.getY()==carY+1)){
		hp++;
		hea.killX();
	}

	usleep(1000000/speed);
	if(hea.timeToDie())
		{
		hea.clear();
		int r=std::rand()%5;
		r+=7;
		usleep(10000000/r);
		r=std::rand()%(WINHEIGHT-3);
		int rr=std::rand()%20;
		hea.reCreate(rr+WINWIDTH-20,r+2);
		}
	}
	hea.clear();
}


void decMovement(Decoration dec){

	while(!dead){
	dec.clear();
	dec.update();
	dec.draw();

	usleep(1000000/speed);
	if(dec.timeToDie())
	{
		dec.clear();
		int r=std::rand()%5;
		r+=7;
		usleep(10000000/r);
		r=std::rand()%(10);
		int rr=std::rand()%20;
		dec.reCreate(rr+WINWIDTH-20,r+WINHEIGHT+5);
	
	}
	}
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
	std::thread t0e(obsMovement,std::ref(obs));
	std::thread t0f(obsMovement,std::ref(obs));

	std::thread d0(rewMovement,std::ref(rew));
	std::thread d1(rewMovement,std::ref(rew));

	std::thread h0(heaMovement,std::ref(hea));

	std::thread t1(repeat,std::ref(car));
	std::thread t2(readbutton);
	std::thread t3(frameMovement);
	std::thread t4(raiseSpeed);
	std::thread t5(decMovement,std::ref(gagduz));

	t0.join();
	t0a.join();
	t0b.join();
	t0c.join();
	t0d.join();
	t0e.join();
	t0f.join();

	d0.join();
	d1.join();

	h0.join();

	t1.join();
	t2.join();
	t3.join();
	t4.join();
	t5.join();

	endwin();

	std::fstream myf("rekord.txt",std::ios_base::in);
	int das;
	myf >> das;

	std::cout << "Punkciory: " << speed << "\n";
	
	if (speed>das)
	{
		std::cout << "POBILES REKORD!\n";
		std::ofstream myfile;
		myfile.open("rekord.txt");
		myfile << speed;
		myfile.close();	
	}
	else
	{
		std::cout << "Rekord: " << das << "\n";
	}
	
	return speed;
}

