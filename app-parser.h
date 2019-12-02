typedef struct {

        char * name;
        char * comment;
        char * exec;
        char * icon;
	char * tags;
        
} App;

//void ParseLine(App *app, char * line);
App * ParseApp (char * path);
