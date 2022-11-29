#include "raylib.h"

//---------------------------------------------------------------------------------
// Definiciones
///////////////////////////////////////////////////////////////
#define CANTIDAD_DISPAROS 50
#define ENEMIGOS_MAXIMOS 50
///////////////////////////////////////////////////////////////
#define PRIMERA_RONDA 10
#define SEGUNDA_RONDA 20
#define TERCERA_RONDA 30
#define CUARTA_RONDA 40
#define QUINTA_RONDA 50
//////////////////////////////////////////

//Definición de tipos y estructuras

typedef enum { PRIMERO = 0, SEGUNDO, TERCERO, CUARTO, QUINTO } RondaEnemiga;

typedef struct level1sound{
    Sound nivel1;
} level1sound;

typedef struct level2sound{
    Sound nivel2;
} level2sound;

typedef struct level3sound{
    Sound nivel3;
} level3sound;

typedef struct level4sound{
    Sound nivel4;
} level4sound;

typedef struct level5sound{
    Sound nivel5;
} level5sound;

typedef struct victoriasound{
    Sound victoria;
} victoriasound;

typedef struct golpeSound{
    Sound golpe;
} golpeSound;

typedef struct Player{
    Texture2D nave;
    Rectangle rec;
    Vector2 vel;
    Color color;
} Player;

typedef struct Enemy{
    Texture2D enemigo;
    Rectangle rec;
    Vector2 vel;
    bool active;
    Color color;
} Enemy;

typedef struct Disparo{
    Rectangle rec;
    Vector2 vel;
    bool active;
    Color color;
    Sound sound;
} Disparo;

//FONDOS DEL JUEGO//
typedef struct fondo1{
    Texture2D fondo1;
} fondo1;

typedef struct fondo2{
    Texture2D fondo2;
} fondo2;

typedef struct fondo3{
    Texture2D fondo3;
} fondo3;


// VARIALES GLOBALES
//------------------------------------------------------------------------------------
static const int screenWidth = 1280;
static const int screenHeight = 720;
/// //
static bool gameOver = false;
static bool pausa =  false;
static int score = 0;
static bool victory = false;

static Player player = { 0 };
static Enemy enemy[ENEMIGOS_MAXIMOS] = { 0 };
static Disparo disparo[CANTIDAD_DISPAROS] = { 0 };
static RondaEnemiga ronda = { ronda};

/// //Declarando sonidos////
static level1sound sonido1 = {sonido1};
static level2sound sonido2 = {sonido2};
static level3sound sonido3={sonido3};
static level4sound sonido4={sonido4};
static level5sound sonido5={sonido5};
static golpeSound golpe={golpe};
static victoriasound victoria={victoria};

//Declarando fondos 
static fondo1 fondoa={fondoa};
static fondo2 fondob={fondob};
static fondo3 fondoc={fondoc};

static int alcance = 0;
static float contador = 0.0f;

static int EnemigosActivos = 0;
static int enemiesKill = 0;
static bool activador = false;

//Scroll del fondo//
float scrollingBack = 0.0f;
    float scrollingMid = 0.0f;
    float scrollingFore = 0.0f;


// DECLARACION DE FUNCIONES 

static void InitGame(void);         
static void UpdateGame(void);     
static void DrawGame(void);       
static void UnloadGame(void);       
static void UpdateDrawFrame(void);  

// El Main
int main(void)
{
    InitWindow(screenWidth, screenHeight, "Nazca Invaders");
    InitAudioDevice();
    InitGame();

   //Sonido del juego//
    PlaySound(sonido1.nivel1);
    
    //frames por segundo del juego
    SetTargetFPS(60);
    //--------------------------------------------------------------------------------------

    // loop del juego
    while (!WindowShouldClose())    
    {
     
        //Mostrar el movimiento del fondo
        UpdateDrawFrame();
        scrollingBack -= 0.1f;
        scrollingMid -= 0.5f;
        scrollingFore -= 1.0f;

        

        // La textura se escala 2 veces
        if (scrollingBack <= -fondoa.fondo1.width*2) scrollingBack = 0;
        if (scrollingMid <= -fondob.fondo2.width*2) scrollingMid = 0;
        if (scrollingFore <= -fondoc.fondo3.width*2) scrollingFore = 0;
        //----------------------------------------------------------------------------------
    }

    //--------------------------------------------------------------------------------------
    //CARGA DE ARCHIVOS
    UnloadGame();         
    CloseAudioDevice();

    CloseWindow();        
    
    //--------------------------------------------------------------------------------------

    return 0;
}

//------------------------------------------------------------------------------------
   
//------------------------------------------------------------------------------------

// Iniciar variables
void InitGame(void)
{

    //fondos//
    fondoa.fondo1 = LoadTexture("textures/fondo.png");
    fondob.fondo2 = LoadTexture("textures/medio.png");
    fondoc.fondo3 = LoadTexture("textures/frente.png");
    

  

    //



    //sonidos

    sonido1.nivel1 = LoadSound("sounds/nivel1.mp3");
    sonido2.nivel2 = LoadSound("sounds/nivel2.mp3");
    sonido3.nivel3 = LoadSound("sounds/nivel3.mp3");
    sonido4.nivel4 = LoadSound("sounds/Nivel4.mp3");
    sonido5.nivel5 = LoadSound("sounds/Nivel5.mp3");
    golpe.golpe = LoadSound("sounds/Golpe.mp3");
    victoria.victoria = LoadSound("sounds/Victoria.mp3");

    // Iniciar variables de juegos
    alcance = 0;
    pausa = false;
    gameOver = false;
    victory = false;
    activador = false;
    ronda = PRIMERO;
    EnemigosActivos = PRIMERA_RONDA;
    enemiesKill = 0;
    score = 0;
    contador = 0;

    // Inicializador jugador
    player.nave = LoadTexture("textures/gallito.png");
    player.rec.x =  20;
    player.rec.y = 340;
    player.rec.width = player.nave.width;
    player.rec.height = player.nave.height;
    player.vel.x = 5;
    player.vel.y = 5; 
    player.color = WHITE;

    // Inicializador de enemigos
    for (int i = 0; i < ENEMIGOS_MAXIMOS; i++)
    {

        enemy[i].enemigo = LoadTexture("textures/textura.png");
        enemy[i].rec.width = enemy[i].enemigo.width;
        enemy[i].rec.height = enemy[i].enemigo.height;
        enemy[i].rec.x = GetRandomValue(screenWidth, screenWidth + 1000);
        enemy[i].rec.y = GetRandomValue(0, screenHeight - enemy[i].rec.height);
        enemy[i].vel.x = 5;
        enemy[i].vel.y = 5;
        enemy[i].color = WHITE;
        enemy[i].active = true;
       
        
    }

    // Inicializador de disparos
    
    for (int i = 0; i < CANTIDAD_DISPAROS; i++)
    {

        disparo[i].sound = LoadSound("sounds/shot.wav");
        disparo[i].rec.x = player.rec.x;
        disparo[i].rec.y = player.rec.y + player.rec.height/4;
        disparo[i].rec.width = 10;
        disparo[i].rec.height = 5;
        disparo[i].vel.x = 7;
        disparo[i].vel.y = 0;
        disparo[i].active = false;
        disparo[i].color = BLUE;
    }
    
    ///////////////////////////////////////////////////////// /////

   

    
}

 
void UpdateGame(void)
{
    if (!gameOver)
    {
        
        
        
        if (IsKeyPressed('Z')) pausa = !pausa;

        if (!pausa)
        {
           
            
            switch (ronda)
            {
                case PRIMERO:
                {
                   
                    if (!activador) //Regresa la logica a activador, resetea a 0
                    {
                        contador += 0.02f; //Agrega y asigna 0.02 decimales a alpha

                        if (contador >= 1.0f)  //si este llega a 1.0 entonces acctivador = verdadero
                        activador = true; 
                    }

                    if (activador) contador -= 0.02f; //Si activador es activo entonces a contador se le va restando 0.02f

                    if (enemiesKill == EnemigosActivos) //Si enemieskill es equivalente a enemigos activos es decir a 0
                    {
                        enemiesKill = 0; //entonces enemieskill =0

                        for (int i = 0; i < EnemigosActivos; i++) //entonces aparecen mas
                        {
                            if (!enemy[i].active)   //regresa la logica al estado de enemigos para activarlos con true.
                            enemy[i].active = true;
                        }

                        EnemigosActivos = SEGUNDA_RONDA;
                        ronda = SEGUNDO;
                        activador = false;
                        contador = 0.0f;
                    }
                } break;
                case SEGUNDO:
                {
                    if (!activador)
                    {
                        contador += 0.02f;

                        if (contador >= 1.0f) activador = true;
                    }

                    if (activador) contador -= 0.02f;

                    if (enemiesKill == EnemigosActivos)
                    {
                        enemiesKill = 0;

                        for (int i = 0; i < EnemigosActivos; i++)
                        {
                            if (!enemy[i].active) 
                            enemy[i].active = true;
                        }

                        EnemigosActivos = TERCERA_RONDA;
                        ronda = TERCERO;
                        activador = false;
                        contador = 0.0f;
                    }
                } break;
                case TERCERO:
                {
                    if (!activador)
                    {
                        contador += 0.02f;

                        if (contador >= 1.0f) activador = true;
                    }

                    if (activador) contador -= 0.02f;

                    if (enemiesKill == EnemigosActivos)
                    {
                        enemiesKill = 0;

                        for (int i = 0; i < EnemigosActivos; i++)
                        {
                            if (!enemy[i].active) enemy[i].active = true;
                        }

                        EnemigosActivos = CUARTA_RONDA;
                        ronda = CUARTO;
                        activador = false;
                        contador = 0.0f;
                    }
                } break;
                case CUARTO:
                {
                    if (!activador)
                    {
                        contador += 0.02f;

                        if (contador >= 1.0f) activador = true;
                    }

                    if (activador) contador -= 0.02f;

                    if (enemiesKill == EnemigosActivos)
                    {
                        enemiesKill = 0;

                        for (int i = 0; i < EnemigosActivos; i++)
                        {
                            if (!enemy[i].active) enemy[i].active = true;
                        }

                        EnemigosActivos = QUINTA_RONDA;
                        ronda = QUINTO;
                        activador = false;
                        contador = 0.0f;
                    }
                } break;
                case QUINTO:
                {
                    if (!activador)
                    {
                        contador += 0.02f;

                        if (contador >= 1.0f) activador = true;
                    }

                    if (activador) contador -= 0.02f;

                    if (enemiesKill == EnemigosActivos){
                     PlaySound(victoria.victoria);
                     victory = true;
                     
                     }
                } break;
                default: break;
            }

            // Movimiento del jugador
            if (IsKeyDown(KEY_RIGHT)) player.rec.x += player.vel.x;
            if (IsKeyDown(KEY_LEFT)) player.rec.x -= player.vel.x;
            if (IsKeyDown(KEY_UP)) player.rec.y -= player.vel.y;
            if (IsKeyDown(KEY_DOWN)) player.rec.y += player.vel.y;

            // colision
            for (int i = 0; i < EnemigosActivos; i++)
            {
                if (CheckCollisionRecs(player.rec, enemy[i].rec)) gameOver = true;
            }

             // Comportamiento del enemigo
            for (int i = 0; i < EnemigosActivos; i++)
            {
                if (enemy[i].active)
                {
                    
                    enemy[i].rec.x -= enemy[i].vel.x; //suma y asigna posición x al enemigo

                    if (enemy[i].rec.x < 0)
                    {
                       
                        enemy[i].rec.x = GetRandomValue(screenWidth, screenWidth + 1000);
                        enemy[i].rec.y = GetRandomValue(0, screenHeight - enemy[i].rec.height);
                        
                    }
                }
            }

            // Muro invisible
            if (player.rec.x <= 0) player.rec.x = 0;
            if (player.rec.x + player.rec.width >= screenWidth) player.rec.x = screenWidth - player.rec.width;
            if (player.rec.y <= 0) player.rec.y = 0;
            if (player.rec.y + player.rec.height >= screenHeight) player.rec.y = screenHeight - player.rec.height;

            // Disparos
            if (IsKeyDown(KEY_SPACE))
            {
                
                PlaySound(disparo->sound);
                alcance += 5;
          


                for (int i = 0; i < CANTIDAD_DISPAROS; i++)
                {
                    if (!disparo[i].active && alcance%20 == 0) //equivalente
                    {
                        disparo[i].rec.x = player.rec.x+107;
                        disparo[i].rec.y = player.rec.y + player.rec.height/2;
                        disparo[i].active = true;
                        break;
                    }
                }
            }

            // Logica del disparo
            for (int i = 0; i < CANTIDAD_DISPAROS; i++)
            {
                if (disparo[i].active)
                {

                    // movimiento
                    
                    disparo[i].rec.x += disparo[i].vel.x;

                    // Colision
                    for (int j = 0; j < EnemigosActivos; j++)
                    {
                        if (enemy[j].active)
                        {
                            if (CheckCollisionRecs(disparo[i].rec, enemy[j].rec))
                            {
                               PlaySound(golpe.golpe);
                                disparo[i].active = false;
                                enemy[j].rec.x = GetRandomValue(screenWidth, screenWidth + 1000);
                                enemy[j].rec.y = GetRandomValue(0, screenHeight - enemy[j].rec.height);
                                alcance = 0;
                                enemiesKill++;
                                score += 100;
                            }

                            if (disparo[i].rec.x + disparo[i].rec.width >= screenWidth)
                            {
                                disparo[i].active = false;
                                alcance = 0;
                            }
                        }
                    }
                }
            }
        }
    }
    else
    //pantalla de game over
    {  
        if (IsKeyPressed(KEY_ENTER))
        {
            StopSound(sonido1.nivel1);
            StopSound(sonido2.nivel2);
            StopSound(sonido3.nivel3);
            StopSound(sonido4.nivel4);
            StopSound(sonido5.nivel5);
            StopSound(victoria.victoria);
            InitGame();
            PlaySound(sonido1.nivel1);
            gameOver = false;
            
        }
    }
}

// Draw game (one frame)
void DrawGame(void)
{

    
    
    BeginDrawing();
         
    
     // Draw background image twice
            // NOTE: Texture is scaled twice its size
            
            DrawTextureEx(fondoa.fondo1, (Vector2){ scrollingBack, 20 }, 0.0f, 2.0f, WHITE);
            DrawTextureEx(fondoa.fondo1, (Vector2){ fondoa.fondo1.width*2 + scrollingBack, 20 }, 0.0f, 2.0f, WHITE);
            

            // Draw midground image twice
            DrawTextureEx(fondob.fondo2, (Vector2){ scrollingMid, 20 }, 0.0f, 2.0f, WHITE);
            DrawTextureEx(fondob.fondo2, (Vector2){ fondob.fondo2.width*2 + scrollingMid, 20 }, 0.0f, 2.0f, WHITE);

            // Draw foreground image twice
            DrawTextureEx(fondoc.fondo3, (Vector2){ scrollingFore, 70 }, 0.0f, 2.0f, WHITE);
            DrawTextureEx(fondoc.fondo3, (Vector2){ fondoc.fondo3.width*2 + scrollingFore, 70 }, 0.0f, 2.0f, WHITE);

         
            DrawText("Diego Alonso Miñano Lavado", screenWidth - 180, screenHeight - 20, 10, RAYWHITE);

        ClearBackground(CLITERAL(Color){0, 188, 244});

        if (!gameOver)
        {

         DrawTexture(player.nave, player.rec.x,player.rec.y,player.color);
         
            if (ronda == PRIMERO){
             DrawText("PRIMER NIVEL", screenWidth/2 - MeasureText("PRIMER NIVEL", 40)/2, screenHeight/2 - 40, 40, Fade(RED, contador));
         
              
            
             
             PlaySound(sonido2.nivel2);
            }
            else if (ronda == SEGUNDO ){

            StopSound(sonido1.nivel1);
            DrawText("SEGUNDO NIVEL", screenWidth/2 - MeasureText("SEGUNDO NIVEL", 40)/2, screenHeight/2 - 40, 40, Fade(BLUE, contador));
            PlaySound(sonido3.nivel3);
           

            }
            else if(ronda == TERCERO){
                StopSound(sonido2.nivel2);
             DrawText("TERCER NIVEL", screenWidth/2 - MeasureText("TERCER NIVEL", 40)/2, screenHeight/2 - 40, 40, Fade(BROWN, contador));
             PlaySound(sonido4.nivel4);
             
            } 
            else if (ronda == CUARTO){
            StopSound(sonido3.nivel3);
            DrawText("CUARTO NIVEL", screenWidth/2 - MeasureText("CUARTO NIVEL", 40)/2, screenHeight/2 - 40, 40, Fade(YELLOW, contador));
            PlaySound(sonido5.nivel5);
           
            
            }
            else if (ronda == QUINTO){
                StopSound(sonido4.nivel4);
                 DrawText("QUINTO NIVEL", screenWidth/2 - MeasureText("QUINTO NIVEL", 40)/2, screenHeight/2 - 40, 40, Fade(VIOLET, contador));
                 
                 
            }

            for (int i = 0; i < EnemigosActivos; i++)
            {
                if (enemy[i].active) DrawTexture(enemy[i].enemigo,enemy[i].rec.x,enemy[i].rec.y,enemy->color);;
            }

            for (int i = 0; i < CANTIDAD_DISPAROS; i++)
            {
                if (disparo[i].active) DrawRectangleRec(disparo[i].rec, disparo[i].color);
            }

            DrawText(TextFormat("%04i", score), 20, 20, 40, GRAY);

            if (victory){

                StopSound(sonido5.nivel5);
                 DrawText("GANASTE!", screenWidth/2 - MeasureText("GANASTE!", 40)/2, screenHeight/2 - 40, 40, BLACK);
                

            }

            if (pausa) DrawText("PAUSA", screenWidth/2 - MeasureText("PAUSA", 40)/2, screenHeight/2 - 40, 40, GRAY);
        }
        else DrawText("PULSA [ENTER] PARA INTENTARLO DE NUEVO", GetScreenWidth()/2 - MeasureText("PULSA [ENTER] PARA INTENTARLO DE NUEVO", 20)/2, GetScreenHeight()/2 - 50, 20, WHITE);

    EndDrawing();
}

// Unload game variables
void UnloadGame(void)
{
     //carga de arvhivos
   
   UnloadSound(disparo->sound);
   UnloadSound(sonido1.nivel1);
   UnloadSound(sonido2.nivel2);
   UnloadSound(sonido3.nivel3);
   UnloadSound(sonido4.nivel4);
   UnloadSound(sonido5.nivel5);
   UnloadSound(victoria.victoria);
   UnloadSound(golpe.golpe);
   //texturas//
   UnloadTexture(player.nave);
   UnloadTexture(enemy->enemigo);
//FONDOS
   UnloadTexture(fondoa.fondo1);  
    UnloadTexture(fondob.fondo2);   
    UnloadTexture(fondoc.fondo3);


   
   
}

// Update and Draw (one frame)
void UpdateDrawFrame(void)
{
    UpdateGame();
    DrawGame();          
    
}