#include"pleiku.h"
bool gameOver = false ;
vector <int> highScore ;


int score ;
SDL_Texture* initBg(SDL_Renderer* renderer){
    string s1 = "./pictures//bgg.png";
    SDL_Texture* bg = loadTexture(s1,renderer);
    SDL_RenderCopy(renderer, bg,NULL,NULL);
}

void phatAmThanh(const char* s){
        SDL_AudioSpec wavSpec ;
        Uint32 wavLength;
        Uint8* wavBuffer ;
        SDL_Init(SDL_INIT_AUDIO);
        SDL_LoadWAV(s, &wavSpec, &wavBuffer, &wavLength);

        SDL_AudioDeviceID deviceId = SDL_OpenAudioDevice(NULL,0, &wavSpec, NULL, 0);
        SDL_QueueAudio(deviceId, wavBuffer, wavLength);
        bool is_paused = false;
        SDL_PauseAudioDevice(deviceId, is_paused);
}
void initDrumStick(drumStick &ds, SDL_Renderer* renderer){

    string s = "./pictures//drumStick.png";
    ds.init(renderer,s);
    ds.changeImageSize(30,30);
    ds.setSpeed(4);
}
void initRocket(object &rk, SDL_Renderer* renderer){
    rk.setHp(2000);
    rk.setSpeed(3);
    rk.changeImageSize( 5,5);
    rk.x = 600 - rk.frame.w/2 ;
    rk.y =  750 - rk.frame.h ;
    rk.setXY(rk.x,rk.y); /// vi tri ban dau
    rk.update();
}
void initHp(object &hp, SDL_Renderer* renderer){
    string s = "./pictures//hp.png";
    hp.init(renderer, s);
    hp.changeImageSize(15,15);

}
void effect (SDL_Renderer* &renderer, const object &v){
    /// truyen toa do vao
    string s1 = "./pictures//effect1.png";
    object effect1 ;
    effect1.changeImageSize(4,4);
    effect1.init(renderer, s1);

    /// set toa do sao cho effect chinh giua bubble
    int x = v.frame.x + v.frame.w/2 - effect1.frame.w/2 ;
    int y = v.frame.y + v.frame.h/2 - effect1.frame.h/2;


    effect1.setXY(x,y);
    effect1.update();
}
void bossDropEgg(egg& e,chicken &boss,const int &cnt ){
    if(cnt % 200 == 0 ){
        e.frame.x = boss.frame.x + boss.frame.w/2-e.frame.w/2 ;
        e.frame.y = boss.frame.y + boss.frame.h/2-e.frame.h/2 ;

    }
    if(e.frame.y < 653){
        e.falling();

    }
    else{
        e.setXY(-100,-100);

    }
}
void dropEgg(int real_imp_time,vector <chicken> &v,vector<egg>&eggs){
    if(v.size()!=0){
                for(int j = 0; j < 5; j++){
                    int i = rand()%v.size();
                    if(real_imp_time % (600+50*i) == 0 && real_imp_time > 10 ){

                        eggs[i].frame.x = v[i].frame.x + v[i].frame.w/2 - eggs[i].frame.w/2 ;
                        eggs[i].frame.y = v[i].frame.y + v[i].frame.y/2 - eggs[i].frame.h/2 ;

                    }
                }
        }

        for(int i =0; i < eggs.size(); i++){
            if(eggs[i].frame.y < 653){
                eggs[i].falling();
            }
            else{
                eggs[i].setXY(-100,-100); /// de no khong hien thi duoc nua
                /// va cai rk cung khong cham vao duoc
            }
        }
}
void flying(SDL_Renderer *renderer, bool& toRight,vector <chicken> &v1, vector<chicken>&v2){
    for(int i = 0; i <v1.size(); i++){
            v1[i].floating(toRight);
            ///v[i].animation(path);
            v1[i].update();
        }
    for(int i = 0; i < v2.size(); i++){
        v2[i].floating2(toRight);
        v2[i].update();
    }

}
void handleEvent(SDL_Event &g_event,float& deltaX,float& deltaY, int &temp, int & temp2, vector <object> &bullets, object spaceShip){
    if(SDL_PollEvent(&g_event)){

            switch(g_event.type){
                case SDL_KEYDOWN:{
                    switch(g_event.key.keysym.sym ){
                        case SDL_QUIT:{
                            SDL_Quit();
                            break;
                        }
                        case SDLK_UP:{ /// di len thi y giam

                            deltaX = 0 ;
                            deltaY = -1 ;
                            break ;
                        }
                        case SDLK_DOWN:{/// di xuong thi y tang
                            deltaX = 0 ;
                            deltaY = 1 ;
                            break ;
                        }
                        case SDLK_LEFT:{/// di sang trai thi x giam
                            deltaX = -1 ;
                            deltaY = 0 ;
                            break;
                        }
                        case SDLK_RIGHT:{/// di sang phai thi x tang
                            deltaX = 1 ;
                            deltaY = 0 ;
                            break ;
                        }
                        case SDLK_SPACE:{ /// ban ra tia
                            temp = 1;

                        break ;
                        }
                        case SDLK_TAB:{

                            object bullet ;
                            string s = "./pictures//bullet.png";
                            ///bullet.changeImageSize(9,7);
                            bullet.init(spaceShip.renderer, s);
                            bullet.setXY(spaceShip.frame.x + spaceShip.frame.w/2 - bullet.frame.w/2, spaceShip.frame.y + spaceShip.frame.h/2-spaceShip.frame.h/2);
                            bullets.push_back(bullet);

                            Sound::GetGame()->LoadEffect("thunderSound","./sound//thunderSound.wav");
                            Sound::GetGame()->PlayEffect("thunderSound");

                        break ;
                        }
                    }

                }
            }
        }
}
void ThunderAndChicken(int &temp,object obj[],vector <drumStick> &drs,vector <vector<chicken>> &chickens,SDL_Renderer* &renderer,drumStick ds ){
    /// between thunder and chicken
    if(temp == 1){
                ///phatAmThanh("./sound//thunderSound.wav");
                Sound::GetGame()->LoadEffect("thunderSound","./sound//thunderSound.wav");
                Sound::GetGame()->PlayEffect("thunderSound");
                int a = obj[1].frame.x + (obj[1].frame.w-obj[2].frame.w)/2;
                int b = obj[1].frame.y - obj[2].frame.h+2;
                obj[2].setXY(a,b);
                obj[2].update();
                    for(int k = 0; k <=1 ; k++){
                        for(int i = 0; i <chickens[k].size(); i++){

                            if(SDL_HasIntersection(&obj[2].frame,&chickens[k][i].frame)){
                            Sound::GetGame()->LoadEffect("chickenSound","./sound//chickenSound.wav");
                            Sound::GetGame()->PlayEffect("chickenSound");

                            effect(renderer, chickens[k][i]);
                            int x1 =chickens[k][i].frame.x + chickens[k][i].frame.w/2 -ds.frame.w/2;
                            int y1 =chickens[k][i].frame.y + chickens[k][i].frame.h/2 -ds.frame.h/2;
                            ds.setXY(x1, y1);
                            drs.push_back(ds);
                            chickens[k].erase(chickens[k].begin()+i);
                            i--;

                            }
                        }
                    temp = 0;
                    }

    }
}

void drumstickAndSpaceship(vector <drumStick> &drs, object &spaceship, int &mark){
     for(int i = 0; i < drs.size(); i++){

                if(drs[i].frame.y < 690){
                    drs[i].frame.y += drs[i].speed ;
                    drs[i].update();
                    if(SDL_HasIntersection(&spaceship.frame,&drs[i].frame)){
                        Sound::GetGame()->LoadEffect("recoverSound","./sound//recoverSound.wav");
                        Sound::GetGame()->PlayEffect("recoverSound");
                        drs.erase(drs.begin()+i);
                        mark+=10;

                    }
                }
                else{
                    /// nay dung ma nhi
                    drs.erase(drs.begin() + i);
                    i--;
                }

        }

}
void hpRender(vector<egg> &eggs, vector <object> &hps, object &spaceship){

    for(int i = 0; i < eggs.size(); i++){
            if(SDL_HasIntersection(&spaceship.frame,&eggs[i].frame)){
                ///phatAmThanh("./sound//hurtSound.wav");
                Sound::GetGame()->LoadEffect("hurtSound","./sound//hurtSound.wav");
                Sound::GetGame()->PlayEffect("hurtSound");
                eggs[i].setXY(-1000,-1000);
                if(hps.size()==0){
                    gameOver = true ;
                }
                else{
                    hps.erase(hps.begin());

                }
            }
    }

    for(int i = 0; i < hps.size(); i++){
        hps[i].setXY(980+hps[i].frame.w*i,10);
        hps[i].update();
    }

}
void recover(vector <object> &hps, object &hp, object &spaceShip,const int &cnt){
   if(cnt % 1000 == 0 && cnt > 1000){


            ///recover(hps ,renderer,real_imp_time,hp, obj[1], cnt);
            int i = rand()%8;
            int u = rand()%10;
            hp.setXY(100*i, 50*u);

        }
        hp.update();
        if(SDL_HasIntersection(&spaceShip.frame, &hp.frame) ){
            hp.setXY(-100,-100);
            if(hps.size()<5){
                hps.push_back(hp);
            }
            Sound::GetGame()->LoadEffect("recoverSound","./sound//recoverSound.wav");
            Sound::GetGame()->PlayEffect("recoverSound");

    }
}
void showTime(const int &real_imp_time, textObject &time_game,SDL_Renderer* &renderer,TTF_Font* font_time ){
        string str_time = "Time: ";
        Uint32 time_val = real_imp_time/1000;
        Uint32 val_time = 120 - time_val ;
        if(val_time <= 0){
            ///Ending();
        }
        else{
            string str_val = to_string(val_time);
            str_time += str_val;
            time_game.SetText(str_time);
            time_game.LoadFromRenderText(font_time, renderer);
            time_game.RenderText(renderer, 500,15, nullptr, 0.0, nullptr, SDL_FLIP_NONE);
        }

}
void showScore(const int& score, textObject &score_text, SDL_Renderer* &renderer, TTF_Font* font_time,const int x, const int y , string s){
        string str_score = s;
        Uint32 score_ = score ;
        string score_val = to_string(score_);
        str_score += score_val ;
        score_text.SetText(str_score);
        score_text.LoadFromRenderText(font_time, renderer);
        score_text.RenderText(renderer, x,y, nullptr, 0.0, nullptr, SDL_FLIP_NONE);
}
void showMessage(textObject &message,SDL_Renderer* &renderer,TTF_Font* font_time, string _s, const int x, const int y){
    string s = _s ;
    message.SetText(s);
    message.LoadFromRenderText(font_time, renderer);
    message.RenderText(renderer, x,y, nullptr, 0.0, nullptr, SDL_FLIP_NONE);
}
/// x = 500, y = 300
void bulletAndBoss(int &temp, object &bullet, object &spaceShip){
    /// lam sao de
    if(temp == 1){
        bullet.frame.x = spaceShip.frame.x + spaceShip.frame.w/2 - bullet.frame.w/2 ;
        bullet.frame.y = spaceShip.frame.y + spaceShip.frame.h/2 - bullet.frame.h/2 - 40 ;
        temp = 0;
    }

    bullet.frame.y -= 3 ;
    /// roi sao nua
    bullet.update();

}
void level2( vector <vector<chicken>> &chickens,
             int &cnt,
             vector<stone> &stones,
             object obj[], string messages[],
             vector <object> &hps){

        if(chickens[0].empty() && chickens[1].empty()){
            for(int i = 0; i < stones.size(); i++){
                stones[i].go(i);
                if(stones[i].frame.x > 1300 || stones[i].frame.y > 850){
                    cnt ++;
                    int x = rand()%10;
                    if(i <10){
                    stones[i].setXY(100*x, -50*i);
                    }
                    else{
                    stones[i].setXY(-10*i, 10*x);

                    }
                }
                if(SDL_HasIntersection(&obj[1].frame,&stones[i].frame)){
                    hps.erase(hps.begin());
                    Sound::GetGame()->LoadEffect("collisionSound","./sound//collisionSound.wav");
                    Sound::GetGame()->PlayEffect("collisionSound");
                    stones[i].setXY(1000,1000);
                }

            }

        }
}
bool handle(object obj[],
             vector <vector<chicken>> chickens,
             SDL_Renderer* renderer,
             SDL_Event g_event, vector<egg> &eggs,
             egg&opla,
             textObject time_game,
             TTF_Font* font_time,
             textObject score_text,
             textObject message)
{
    /// stone
    stone stone1;
    string s = "./pictures//stone.png";
    stone1.init(renderer, s);
    vector<stone> stones;
    stone1.changeImageSize(10,10);
    for(int i = 0; i < 10; i++){
        int x = rand()%10;
        stone1.setXY(100*x, -50*i);
        stones.push_back(stone1);
    }
    for(int i = 10; i < 20; i++){
        int x = rand()%10;
        stone1.setXY(-50*i, 100*x);
        stones.push_back(stone1);
    }

    string messages[4]={"Start", "Level 1", "Level 2", "Final Level"};
    float deltaX, deltaY ;
    int temp = 0;
    bool toRight = true ;
    bool toRight2 = true ;

    /// init ds
    drumStick ds;
    initDrumStick(ds, renderer);

    /// set up timer
    ImpTimer fps_timer;
    fps_timer.start();
    int real_imp_time ;

    /// init hp
    object hp;
    initHp(hp, renderer);

    vector <object> hps ;
    for(int i = 0; i < 5; i++){
        hp.setXY(980+hp.frame.w*i,10);
        hps.push_back(hp);
    }
    score = 0;
    vector <drumStick> drs ;
    int cnt = 0;

    chicken boss ;
    string str_boss = "./pictures//boss.png";
    boss.init(renderer, str_boss);
    boss.changeImageSize(2.5,1);
    boss.setXY(500,100);
    int cnt2 = 0 ;
    object bullet;
    bullet.init(renderer, "./pictures//bullet.png");
    bullet.setXY(500,500);
    bullet.changeImageSize(9,7);
    vector <object> bullets ;
    vector <int> tmps ;
    for(int i = 0; i< 3; i++){
        tmps.push_back(temp);
    }
    int temp2 = 0;
    int kills = 0;
    bool win = false ;
    gameOver = false ;

while(!gameOver && !win ){
        if(hps.size()==0){
            gameOver = true ;
        }

        /// ham lay thoi gian thuc
        real_imp_time = fps_timer.get_ticks();


        /// ga bay, lam roi trung
        flying(renderer,toRight,chickens[0],chickens[1]);

        dropEgg(real_imp_time, chickens[0], eggs);

        dropEgg(real_imp_time, chickens[1], eggs);
        /// va cham giua sam set va ga
        ThunderAndChicken(temp,obj,drs,chickens,renderer,ds );
        /// xu li su kien ban phim
        handleEvent(g_event, deltaX, deltaY, temp, temp2, bullets, obj[1]);
        obj[1].changeXY(deltaX, deltaY);


        ///bg thay doi
        if(obj[0].frame.y+obj[0].frame.h >= 750  && obj[3].frame.y <= 0){
            obj[0].frame.y -= deltaY*obj[1].speed ; /// bg1
            obj[3].frame.y -= deltaY*obj[1].speed ; /// bg2
        }

        /// an dui ga
        drumstickAndSpaceship(drs, obj[1], score);
        /// an trai tim
        recover(hps, hp, obj[1], cnt);

        if(real_imp_time < 1000){
            showMessage(message, renderer, font_time, messages[0], 500,300);
        }
        else if(1000<=real_imp_time && real_imp_time<=2000){
            showMessage(message, renderer, font_time, messages[1], 500, 300);
        }

        /// bat dau level 2

        if(chickens[0].empty() && chickens[1].empty() ){
            cnt2++;
            if(cnt2 < 500){
             Sound::GetGame()->LoadEffect("uplevelSound","./sound//uplevelSound.wav");
            Sound::GetGame()->PlayEffect("uplevelSound");
            showMessage(message, renderer, font_time, messages[2], 500, 300);
            }
            /// o man 2 diem troi theo thoi gian
            if(cnt % 200 == 0){
                score++;
            }
        }

        level2(chickens,cnt,stones,obj, messages, hps);
        ///bat dau level 3 nay
        /// tai sao hps luc erase lai erase tat ca luon nhi
        if(real_imp_time > 41*1000){
            if(real_imp_time < 42*1000){
                     Sound::GetGame()->LoadEffect("uplevelSound","./sound//uplevelSound.wav");
                    Sound::GetGame()->PlayEffect("uplevelSound");
                showMessage(message, renderer, font_time, messages[3], 500, 300);
            }
            for(int i = 0; i < bullets.size(); i++){
                /// con gi nua nhi
                bullets[i].frame.y -= 3 ;
                if(SDL_HasIntersection(&bullets[i].frame, &boss.frame)){

                    Sound::GetGame()->LoadEffect("chickenSound","./sound//chickenSound.wav");
                    Sound::GetGame()->PlayEffect("chickenSound");
                    bullets[i].setXY(0, -1);
                    ///boss.vibrate();
                    score+=10;
                    kills++;

                }

                bullets[i].update();
                if(bullets[i].frame.y < 0){
                    bullets.erase(bullets.begin()+i);
                    i--;
                }

            }
            boss.floating(toRight2);
            bossDropEgg(opla,boss,cnt );
            if(SDL_HasIntersection(&obj[1].frame, &opla.frame)){

                Sound::GetGame()->LoadEffect("hurtSound","./sound//hurtSound.wav");
                Sound::GetGame()->PlayEffect("hurtSound");
                   hps.erase(hps.begin());
                   opla.setXY(-100,-10);

            }

            if(kills <= 20){
                boss.update();
            }
            else{
                object explosion ;
                explosion.init(renderer, "./pictures/explosion.png");
                explosion.changeImageSize(4,4);
                explosion.setXY(boss.frame.x+boss.frame.w/2-explosion.frame.w/2, boss.frame.y+boss.frame.h/2-explosion.frame.h/2);
                explosion.update();
                win = true ;
            }
        }


        showScore(score, score_text, renderer,font_time, 0, 15, "Your score: ");
        showTime(real_imp_time, time_game, renderer, font_time);
        hpRender(eggs,hps,obj[1]);

        obj[1].update();
        Present(renderer);
        if(win || gameOver){

            break;
        }
        obj[0].update();
        obj[3].update();
        cnt++;

    }
    highScore.push_back(score);

    if(win){
        Sound::GetGame()->LoadEffect("winnerSound","./sound//winnerSound.wav");
        Sound::GetGame()->PlayEffect("winnerSound");
        return true ;
    }
    else if(gameOver){
        Sound::GetGame()->LoadEffect("loserSound","./sound//loserSound.wav");
        Sound::GetGame()->PlayEffect("loserSound");
        return false ;
    }

    return false ;

}

void play(){
        SDL_Window* window = NULL ;
        SDL_Renderer* renderer = NULL;
        initSDL(window, renderer);
        bool success = true;
        phatAmThanh("./sound//backgroundSound.wav");
        if(TTF_Init()==-1){

            success = false ;
        }
        TTF_Font* font_time = nullptr;
        font_time = TTF_OpenFont("font//font2.ttf", 40);
        if(font_time == NULL){
            success = false ;
        }
        textObject time_game ;
        textObject score_text;
        textObject message;

        ///time_game.SetColor(255,255,255);
        ///initbg
        object bg;
        string s1 = "./pictures//bg1.png";
        bg.init(renderer,s1);
        bg.frame = {0,0,1200,750};
        bg.update();
        switch(Menu(renderer)){
            case 0 : // play y
                Sound::GetGame()->LoadEffect("clickSound","./sound//clickSound.wav");
                Sound::GetGame()->PlayEffect("clickSound");
                break ;
            case 1:
                Sound::GetGame()->LoadEffect("clickSound","./sound//clickSound.wav");
                Sound::GetGame()->PlayEffect("clickSound");
                return ;


        }
        ///initbg2
        object bg2 ;
        string s0 = "./pictures//bg2.png";
        bg2.init(renderer, s0);
        //int h = SCREEN_HEIGHT;
        bg2.frame = {0,-750, 1200, 750};
        ///init rocket
        object rk;
        string s2 = "./pictures//spaceship0.png";
        rk.init(renderer,s2);
        initRocket(rk, renderer);


        ///init thunder
        object thunder;
        string s3 = "./pictures//thunder.png";
        thunder.init(renderer, s3);
        thunder.changeImageSize(0.16,9);

        ///init chicken
        string s4 ="./pictures//chicken007.png";
        chicken bubble;
        bubble.init(renderer, s4);
        bubble.changeImageSize(5,5);
        vector <chicken> v;
        vector <vector<chicken>> chickens ;
        /// hay lai tao 1 cai mang nua nhi
        for(int i = 0; i < 10; i++){
            bubble.setXY(20+ (bubble.frame.w+10)*(i+1), 100+20*i);
            v.push_back(bubble);
        }
        /// init chicken2
        string s9 ="./pictures//chicken008.png";
        chicken ck2;
        ck2.init(renderer, s9);
        ck2.changeImageSize(7,7);
        vector<chicken> v2 ;
        for(int i = 0; i <10; i++){
            ck2.setXY(20+ (ck2.frame.w+10)*(i+1), 100+20*i);
            v2.push_back(ck2);
        }
        for(int i = 0; i < 2; i++){
            if(i % 2 ==0 ){
                chickens.push_back(v);
            }
            else{
                chickens.push_back(v2);
            }

        }
        /// sau khi choi game xong, minh muon no render len renderer, xong roi moi

        ///init egg
        egg e ;
        string s5 = "./pictures//egg.png";
        vector <egg> eggs ;
        for(int i = 0; i < 10; i++){
            e.init(renderer, s5);
            e.changeImageSize(18,18);
            e.setXY(100,100);
            eggs.push_back(e);
        }
        ///init opla
        object character[4]={bg, rk, thunder,bg2};
        string s6 = "./pictures//opla.png";
        egg opla ;
        opla.init(renderer, s6);
        opla.changeImageSize(17,17);
        opla.setXY(-100,100);
        ///handle
        SDL_Event g_event ;

        if(handle(character,chickens, renderer, g_event, eggs, opla, time_game,font_time,score_text, message)){
            string s1 = "Sugoiii";
            string s2 = "You win!";
            showMessage(message, renderer, font_time,s1, 300, 250);
            showMessage(message, renderer, font_time,s2, 300, 300);

        }
        else{
             string s1 = "You lose :<";
            showMessage(message, renderer, font_time, s1, 300, 300);

        }
        showScore(score, score_text, renderer, font_time, 300, 350, "Your score: ");
        int x = *max_element(highScore.begin(), highScore.end());
        showScore(x, score_text, renderer, font_time, 300, 400, "Highest score: ");

        showMessage(message, renderer, font_time, "You want to replay?", 300, 450);
        Present(renderer);
/// no choi lai dc moi 1 lan
        switch(Menu2(renderer)){
        case 0:
            Sound::GetGame()->LoadEffect("clickSound","./sound//clickSound.wav");
            Sound::GetGame()->PlayEffect("clickSound");
            return ;
        case 1:

            Sound::GetGame()->LoadEffect("clickSound","./sound//clickSound.wav");
            Sound::GetGame()->PlayEffect("clickSound");
/// goi lai ham handle
            play();
        }

        return ;
}
