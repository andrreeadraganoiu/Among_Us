#include "utils.h"

void* (* Abilities[4])(void* x) = {RotateMatrix, DecodeString, InvertArray, KillPlayer};

char *fromEnumtoString(PlayerRoles playerRole)
{
	char *str = calloc(MAX_LEN_STR_ATR, sizeof(char));
	switch(playerRole)
	{
		case Rotator:
			str = strcpy(str, "Rotator");
			break;
		case Decoder:
			str = strcpy(str, "Decoder");
			break;
		case Invertor:
			str = strcpy(str, "Invertor");
			break;
		case Impostor:
			str = strcpy(str, "Impostor");
			break;
	}
	return str;
}

// Task 1

void *RotateMatrix(void *input)
{
	int n, i, j;
	n = *((int*)input); //dimensiunea matricei
	
	char *string = (char *) calloc(MAX_LEN_STR_OUT, sizeof(char)); 
	for (i = 1 ; i <= n ;i++)
	{
		for (j = 1 ; j <= n ; j++)
			sprintf(string + strlen(string), "%d%d ", n - j + 1, i);   //construiesc elementele pseudomatricei
		if(i < n)  							
			sprintf(string + strlen(string),"%c",'\n');	

	}

	return (void *)string;
}

// Task 2
void *DecodeString(void *input)
{
	int s = 0;
	char *sum = (char *) calloc(MAX_LEN_STR_OUT, sizeof(char)); //sirul in care pun suma
	char *p = strtok(input,"_");
	while(p)
	{
		s = s + atoi(p);
		p = strtok(NULL, "_");
	}
	sprintf(sum, "%d", s);

	return (void *)sum;
}

//Task 3
void swap(int *a, int *b)
{
	int aux = *a;
	*a = *b;
	*b = aux;
}
void *InvertArray(void *input)
{

	int *p = (int*)input;  
	int n = p[0];  //numarul de elemente
	if (n % 2 == 0)
	{
		for (int i = 1; i < n ; i = i + 2)
			swap(&p[i], &p[i+1]);

	}
	else
	{
		for (int i = 1; i <= n/2; i++)
			swap(&p[i], &p[n - i + 1]);
	}
	
	char *input1 = (char *) calloc(MAX_LEN_STR_OUT, sizeof(char)); //sirul in care pun elementele vectorului
	for (int i = 1; i <= n; i++)      // p[0] este numarul de elemente
			sprintf(input1 + strlen(input1), "%d ", p[i]);
	return input1;

}

//Task 4
Player *allocPlayer()
{
	Player *newPlayer = (Player *) calloc(1, sizeof(Player));
	if (!newPlayer)
	{
		exit(1);
	}
	newPlayer->name = (char *) calloc(MAX_LEN_STR_ATR, sizeof(char));
	newPlayer->color = (char *) calloc(MAX_LEN_STR_ATR, sizeof(char));
	newPlayer->hat = (char *) calloc(MAX_LEN_STR_ATR, sizeof(char));
	newPlayer->alive = 1;

	return newPlayer;
}

//Task 4
Game *allocGame()
{
	Game *newGame = (Game *) calloc(1, sizeof(Player));
	if(!newGame)
	{
		exit(1);
	}
	newGame->name = (char *)calloc(MAX_LEN_STR_ATR, sizeof(char));

	return newGame;
}

PlayerRoles fromStringtoEnum(char str[]) //functie pentru transformarea din string in enum
{
	PlayerRoles role;
	if(strcmp("Rotator", str) == 0)
		role = Rotator;
	if(strcmp("Decoder", str) == 0)
		role = Decoder;
	if(strcmp("Invertor", str) == 0)
		role = Invertor;
	if(strcmp("Impostor", str) == 0)
		role = Impostor;
	return role;
}
//Task 5
Player *ReadPlayer(FILE *inputFile)
{
	char sep[] = ",() ";
	char *role = (char *)calloc(MAX_LEN_STR_ATR, sizeof(char));
	char *location_line = (char *)calloc(MAX_LEN_STR_ATR, sizeof(char)); //linia cu coordonatele pozitiilor
	if(!location_line)
		exit(1);
	Player *newPlayer = allocPlayer();
	fscanf(inputFile, "%s", newPlayer->name);
	fscanf(inputFile, "%s", newPlayer->color);
	fscanf(inputFile, "%s", newPlayer->hat);
	newPlayer->indexOfLocation = 0;
	fscanf(inputFile, "%d", &newPlayer->numberOfLocations);

	newPlayer->locations = (Location*) calloc(newPlayer->numberOfLocations, sizeof(Location)); //aloc structura de pozitii
	if(!newPlayer->locations)
		exit(1);
	for (int i = 0; i < newPlayer->numberOfLocations; i++)
	{
		fscanf(inputFile, "%s", location_line);
	
		char *p = strtok(location_line, sep);
		while(p)
		{
			newPlayer->locations[i].x = atoi(p);
			p = strtok(NULL, sep);
			newPlayer->locations[i].y = atoi(p);
			p = strtok(NULL, sep);
		}
	}
	free(location_line);
	fscanf(inputFile, "%s", role);
	newPlayer->playerRole = fromStringtoEnum(role); // am nevoie de enum
	free(role);
	newPlayer->ability = Abilities[newPlayer->playerRole];

	return newPlayer;
}

// Task 5
Game *ReadGame(FILE *inputFile)
{
	Game *newGame = allocGame();
	fscanf(inputFile, "%s", newGame->name);
	fscanf(inputFile, "%d", &newGame->killRange);
	fscanf(inputFile, "%d", &newGame->numberOfCrewmates);

	newGame->crewmates = (Player **) calloc(newGame->numberOfCrewmates, sizeof(Player *)); //aloc matricea formata din structurile jucatorilor
	if(!newGame->crewmates)
		exit(1);
	for(int i = 0; i < newGame->numberOfCrewmates; i++)
		newGame->crewmates[i] = ReadPlayer(inputFile); //citesc fiecare structura a jucatorilor pe linie
	newGame->impostor = ReadPlayer(inputFile); //citesc structura impostorului
	return newGame;
}

// Task 6
void WritePlayer(Player *player, FILE *outputFile)
{	
	char *player_role = fromEnumtoString(player->playerRole); //transform din enum in string
	fprintf(outputFile, "Player %s with color %s, hat %s and role %s has entered the game.\n", player->name, player->color, player->hat, player_role);
	free(player_role);
	fprintf(outputFile, "Player's locations: ");
	for(int i = 0; i < player->numberOfLocations; i++)
	{
		fprintf(outputFile, "(%d,%d) ", player->locations[i].x, player->locations[i].y);
	}
	return;
}

// Task 6
void WriteGame(Game *game, FILE *outputFile)
{
	fprintf(outputFile, "Game %s has just started!\n", game->name);
	fprintf(outputFile, "\tGame options:\n");
	fprintf(outputFile, "Kill Range: %d\n", game->killRange);
	fprintf(outputFile, "Number of crewmates: %d\n\n", game->numberOfCrewmates);
	fprintf(outputFile, "\tCrewmates:\n");

	for(int i = 0; i < game->numberOfCrewmates; i++)
	{
		WritePlayer(game->crewmates[i], outputFile);
		fprintf(outputFile, "\n");
	}
	fprintf(outputFile, "\n\tImpostor:\n");
	WritePlayer(game->impostor, outputFile);
	return;
}
int distance_calculator(Player *Crewmate, Location impostor_location) //calculez distanta Manhattan dintre pozitia impostorului si a jucatorului
{
	int distance;
	Location crewmate_location = Crewmate->locations[Crewmate->indexOfLocation];
	distance = abs(crewmate_location.x - impostor_location.x) + abs(crewmate_location.y - impostor_location.y);
	return distance;

}
//Task 7
void *KillPlayer(void *input)
{
	int distance, min;

	Game *newGame = (Game *) input;
	Player *Impostor = newGame->impostor;
	Location impostor_location = Impostor->locations[Impostor->indexOfLocation]; //pozitia impostorului

	Player *Crewmate = newGame->crewmates[0];//consider ca primul jucator are distanta cea mai mica dintre el si impostor, prin urmare el
	Player *Killed = Crewmate;               // este cel ucis in cazul in care distanta lui nu e mai mare decat killRange, verific asta cand afisez           
	min = distance_calculator(Crewmate, impostor_location); 
	
	for(int i = 1; i < newGame->numberOfCrewmates; i++)
	{
		Player *Crewmate = newGame->crewmates[i];
		distance = distance_calculator(Crewmate, impostor_location);

		if ((distance < min && Crewmate->alive == 1) || (Killed->alive == 0))  //calculez distanta fiecarui jucator si o compar cu minimul gasit pana in prezent
		{																		//ultima conditie este pentru task8, daca primul jucator(initializat cu distanta min) este omorat
			Killed = Crewmate;													//Killed preia pentru moment primul jucator alive 
			min = distance;
		}
		else
		{
			if((distance == min && Crewmate->alive ==1) || (Killed->alive ==0))   //iau jucatorul cu indicele mai mare
				if(Killed->indexOfLocation <= Crewmate->indexOfLocation)
				{
					min = distance;
					Killed = Crewmate;
				}
		}

	}
	char *output = calloc(MAX_LEN_STR_OUT, sizeof(char));
	if(min > newGame->killRange || (Killed->alive == 0)) //daca distanta cea mai mica a unui jucator de impostor este mai mare decat killRange impostorul nu a putut omori pe nimeni 
		sprintf(output, "Impostor %s couldn't kill anybody.\n", Impostor->name);
	else
	{
		sprintf(output, "Impostor %s has just killed crewmate %s from distance %d.\n", Impostor->name, Killed->name, min);
		Killed->alive = 0; //daca distanta este suficienta voi face campul lui Killed 0, 
						   //fiind in acelasi timp si Crewmate->alive = 0 pentru ca sunt pointeri
	}
	

	return (void *)output;
}

// Task 8
void CalcuateNextCycleOfGame(Game *game, FILE *outputFile, void **inputMatrix)
{
	for (int i = 0; i < game->numberOfCrewmates; i++)
	{
		if (game->crewmates[i]->indexOfLocation + 1 < game->crewmates[i]->numberOfLocations )  // daca jucatorul nu se afla pe ultima pozitie
			game->crewmates[i]->indexOfLocation++;
		else
		{
			game->crewmates[i]->indexOfLocation = 0;
		}
		
		Location crewmate_location = game->crewmates[i]->locations[game->crewmates[i]->indexOfLocation];
		switch(game->crewmates[i]->alive)
		{	case 1:
				fprintf(outputFile, "Crewmate %s went to location (%d,%d).\n", game->crewmates[i]->name, crewmate_location.x, crewmate_location.y);
				fprintf(outputFile, "Crewmate %s's output:\n", game->crewmates[i]->name);
				char *ability = (char *)game->crewmates[i]->ability(inputMatrix[i]);
				fprintf(outputFile, "%s\n", ability);
				free(ability);
				break;
			case 0:
				fprintf(outputFile, "Crewmate %s is dead.\n", game->crewmates[i]->name);
				break;
		}
	}

		if (game->impostor->indexOfLocation + 1 < game->impostor->numberOfLocations ) //daca impostorul nu se afla pe ultima pozitie
			game->impostor->indexOfLocation++;
		else
		{
			game->impostor->indexOfLocation = 0;
		}
		char *ability = (char *)game->impostor->ability(game);
		Location impostor_location = game->impostor->locations[game->impostor->indexOfLocation];
		fprintf(outputFile, "Impostor %s went to location (%d,%d).\n", game->impostor->name, impostor_location.x, impostor_location.y);
		fprintf(outputFile, "Impostor %s's output:\n", game->impostor->name);
		fprintf(outputFile, "%s", ability);
		free(ability);
		
	
	return;
}

// Task 9
void FreePlayer(Player *player)
{
	free(player->name);
	free(player->color);
	free(player->hat);
	free(player->locations);
	free(player);
	return;
}

// Task 9
void FreeGame(Game *game)
{
	free(game->name);
	for(int i = 0; i < game->numberOfCrewmates; i++)
	{
		FreePlayer(game->crewmates[i]);
	}
	free(game->crewmates);
	FreePlayer(game->impostor);
	free(game);
	return;
}