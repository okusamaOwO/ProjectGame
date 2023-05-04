#include"chicken.h"
    void chicken::floating2(bool &toRight){

        if(toRight){
            frame.x+=1;
            //100*cos(listBoss[i].khung_nv.x*3.14/180) + 100
            frame.y = 50*sin(frame.x*3.14/100)+100;
            if(frame.x + frame.w >= 1200) toRight = false ;


        }
        else{
            frame.x-=1;
            frame.y =50*sin(frame.x*3.14/100)+100;
            if(frame.x == 0) toRight = true;

        }

    }
    void chicken::floating(bool &toRight){
        if(toRight){
            frame.x+=1;
            //100*cos(listBoss[i].khung_nv.x*3.14/180) + 100
            frame.y = 50*cos(frame.x*3.14/100) + 50;
            if(frame.x + frame.w >= 1200) toRight = false ;


        }
        else{
            frame.x-=1;
            frame.y = 50*cos(frame.x*3.14/100) + 50;
            if(frame.x == 0) toRight = true;

        }
    }
    void chicken::falling(){
        frame.y += 4 ;
    }
   /** void chicken::dropEgg(egg& e){
        /// con nay se tha trung lien tuc

    }
    **/
