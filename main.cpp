#include "main.h"

namespace Avatar{
    bool facing_right = true;
    yx pos;

    void draw(){
        mvaddch(pos.y, pos.x, 'P');
        switch(facing_right){
            case true:
                mvaddch(pos.y, pos.x+1, '>');
                break;
            case false:
                mvaddch(pos.y,pos.x-1, '<');
                break;
        }
    }
}

class Projectile{
public:
    yx pos;
    bool active = true;
    bool fired_right;

    Projectile(yx start, bool right){
        pos = start;
        fired_right = right;
    }

    void draw(){
        mvaddch(pos.y, pos.x, '*');
    }

    void advance(){
        switch(fired_right){
            case true:
                pos.x += 1;
                break;
            case false:
                pos.x -= 1;
                break;
        }
        if(pos.x >= COLS || pos.x <= 0){
            active = false;
        }
    }
};

class Enemy{
public:
    yx pos;
    bool active = true;
    bool spawn_on_right;

    Enemy(){
        switch(rand()%2){
            case 0:
                spawn_on_right = true;
                break;
            case 1:
                spawn_on_right = false;
                break;
        }
        if(spawn_on_right){
            pos.x = COLS -1;
        }
        else{
            pos.x = 1;
        }
        pos.y = LINES - 2 - (rand()%10);
    }

    void advance(){
        switch(spawn_on_right){
            case true:
                pos.x -= 1;
                break;
            case false:
                pos.x += 1;
                break;
        }
        if(pos.x >= COLS || pos.x <= 0){
            active = false;
        }
    }

    void draw(){
        mvaddch(pos.y, pos.x, '!');
    }

};

int main(){
    WINDOW* here = initscr();
    srand(2432879);

    int error_counter = 0;
    int score = 0;

    int failures = 0;

    noecho();
    keypad(here, true);
    std::vector<Projectile*> projectiles;
    std::vector<Enemy*> enemies;

    Avatar::pos = {LINES-5, COLS/2};

    int ch = 0;

    int interval = 0;
    int spawn_speed = 512;

    while(true){
        usleep(SPEED_LIMITER);

        if(interval < 1024){
            interval++;
        }
        else{
            interval = 0;
        }

        // Render pass
        clear();
        move(LINES-1, 0);
        hline('#', COLS);
        Avatar::draw();
        mvaddstr(0,0, std::to_string(interval).c_str()); // Interval Counter
        mvaddstr(0,9, std::to_string(error_counter).c_str()); // Error Counter
        // Score
            move(2,0);
            addstr("SCORE: ");
            addstr(std::to_string(score).c_str());
            move(3,0);
            addstr("FAILS: ");
            addstr(std::to_string(failures).c_str());
        //
        for(auto p : projectiles){
            p->draw();
        }
        for(auto e : enemies){
            e->draw();
        }
        //

        // Gravitas
        if( (Avatar::pos.y != LINES-2) && (interval%18 == 0) ){
            Avatar::pos.y += 1;
        }
        //

        // Bullets
        if(interval%6 == 0){
            try{
                for(int p = 0; p < projectiles.size(); p++){
                    projectiles.at(p)->advance();
                    if(!projectiles.at(p)->active){
                        delete projectiles.at(p);
                        projectiles.erase(projectiles.begin() + p);
                    }
                }
            }
            catch(std::exception &e){
                error_counter++;
            }
        }
        //

        // Enemies
        if(interval%12 == 0){
            try{
                for(int e = 0; e < enemies.size(); e++){
                    enemies.at(e)->advance();
                    if(!enemies.at(e)->active){
                        delete enemies.at(e);
                        enemies.erase(enemies.begin() + e);
                        failures++;
                    }
                }
            }
            catch(std::exception &e){
                error_counter++;
            }
        }
        // Enemy-Projectile Collision
            try{
                for(int e = 0; e < enemies.size(); e++){
                    for(int p = 0; p < projectiles.size(); p++){
                        if( abs((enemies.at(e)->pos.x - projectiles.at(p)->pos.x)) <= 1
                        && enemies.at(e)->pos.y == projectiles.at(p)->pos.y){
                            delete enemies.at(e);
                            enemies.erase(enemies.begin() + e);
                            delete projectiles.at(p);
                            projectiles.erase(projectiles.begin() + p);
                            score++;
                            if(score%5 == 0 && spawn_speed > 8){
                                spawn_speed = spawn_speed * 0.95;
                            }
                            
                        }
                    }
                }
            }
            catch(std::exception &enemies){
                error_counter++;
            }

        //Enemy Spawn
        if(interval%spawn_speed == 0){
            enemies.push_back(new Enemy);
        }
        
        nodelay(here, true);
        ch = getch();
        switch(ch){
            case KEY_LEFT:
                Avatar::facing_right = false;
                if(Avatar::pos.x > 0){
                    Avatar::pos.x -= 1;
                }
                break;
            case KEY_RIGHT:
                Avatar::facing_right = true;
                if(Avatar::pos.x < COLS){
                    Avatar::pos.x += 1;
                }
                break;
            case KEY_UP:
                Avatar::pos.y -= 3;
                break;
            case ' ':
                projectiles.push_back(new Projectile(Avatar::pos, Avatar::facing_right));
                break;
            default:
                break;
        }
    }
}