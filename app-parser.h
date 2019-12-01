typedef struct App {

        char * name;
        char * comment;
        char * exec;
        char * icon;
        
} App;

//void ParseLine(App *app, char * line);
App * ParseApp (char * path);
