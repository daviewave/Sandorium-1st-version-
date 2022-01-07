#include <stdio.h>

int player_direction_is_line(char cur_builder_turn, int next_row, int next_column, int game_board_arr[][7]);
int is_blocked(char cur_player, char other_player, int next_row, int next_column, int game_board_arr[][7]);
int player_direction_is_line(char cur_builder_turn, int next_row, int next_column, int game_board_arr[][7]);
void update_tower_levels(char cur_builder_turn, int direction_moved, int next_row, int next_column, int game_board_arr[][7]);
int check_game_over(int game_board_arr[][7]);
int traverse_board(int search, int game_board_arr[][7]);

const int asc_p = 80;
const int asc_a = 65;
const char player = asc_p;
const char computer_player = asc_a;
int previous_value;




void print_board(int game_board_arr[][7])
{
  int zero_counter, four_counter;

  for(int i = 0; i < 7; i++)
  {
      for(int j = 0; j < 7; j++)
      {
        if(game_board_arr[i][j] == 0)
        {
          zero_counter++;
        }
        if(game_board_arr[i][j] == 4)
        {
          four_counter++;
        }
        if(game_board_arr[i][j] == 80)
        {
           game_board_arr[i][j] = player;
           printf("%c ", player);
        }
        else if(game_board_arr[i][j] == 65)
        {
          game_board_arr[i][j] = computer_player;
          printf("%c ", computer_player);
        }
        else
        {
          printf("%i ", game_board_arr[i][j]);  
        }
      }
      printf("\n");
  }
  printf("\n");
}

int get_builder_row_location(char cur_player, int game_board_arr[][7])
{
  for(int i = 0; i < 7; i++)
  {
    for(int j = 0; j < 7; j++)
    {
      if(game_board_arr[i][j] == cur_player)
      {
        return i;
      }
    }
  }
  return 0;
}

int get_builder_column_location(char cur_player, int game_board_arr[][7])
{
  for(int i = 0; i < 7; i++)
  {
    for(int j = 0; j < 7; j++)
    {
      if(game_board_arr[i][j] == cur_player)
      {
        return j;
      }
    }
  }
  return 0;
}

//Allows player to choose their starting space and determines where Ai will start
void starting_spot(int game_board_arr[][7])
{
    int starting_row, starting_column;
    
    printf("\nChoose the row and column you would like your builder to start at: \n");
    printf("Starting row: \n");
    scanf("%i", &starting_row);
    
    if(starting_row < 1 || starting_row > 6)
    {
      printf("You chose a row that is not on the playable part of the game board. Enter a row number from 1-6.\n");
      starting_spot(game_board_arr);
    }
   
    printf("Starting column: \n");
    scanf("%i", &starting_column);
    
    if(starting_row < 1 || starting_row > 6)
    {
      printf("You chose a column that is not on the playable part of the game board. Enter a column number from 1-6.\n");
      starting_spot(game_board_arr);
    }
    previous_value = game_board_arr[starting_row][starting_column];
    game_board_arr[starting_row][starting_column] = player;
    
    printf("This is your starting location: \n");
    print_board(game_board_arr);

    //Determines a starting space for the ai that is adjacent to the player
    if(starting_row < 3)
    {
      game_board_arr[starting_row + 1][starting_column] = computer_player;
    }
    else if(starting_row > 4)
    {
      game_board_arr[starting_row - 1][starting_column] = computer_player;
    }
    else if((starting_row == 3 || starting_row == 4) && starting_column < 3)
    {
      game_board_arr[starting_row][starting_column + 1] = computer_player;
    }
    else if((starting_row == 3 || starting_row == 4) && starting_column > 3)
    {
      game_board_arr[starting_row][starting_column - 1] = computer_player;
    }
    else
    {
      game_board_arr[starting_row + 1][starting_column + 1] = computer_player;
    }

    printf("The computer player will start here: \n");
    print_board(game_board_arr);
}

void print_blocking_error()
{
  printf("\nYou chose a space thats pathway is blocked by the computer player.\n");
  printf("Please chose a space with a straight pathway that is unblocked by the computer player.\n\n");
}

void move_builder(char next_builder, int game_board_arr[][7])
{
  int cur_row, cur_column, next_row, next_column, direction, is_path_blocked;

  printf("\nCurrent board\n");
  print_board(game_board_arr);

  if(next_builder == player)
  {
    //Tells player its their turn and asks coordinate of the row theyd like to move to
    printf("It is your turn to move. Enter the coordinate of the row you would like to move to, 1-6: \n");
    scanf("%i", &next_row);
    if(next_row < 1 || next_row > 6)
    {
      printf("You chose a row that is not on the playable part of the game board. Enter a row number from 1-6.\n");
      move_builder(player, game_board_arr);
    }
    //asks the player the coordinate of the column theyd like to move to
    printf("Enter the coordinate of the column you would like to move to, 1-6: \n");
    scanf("%i", &next_column);
    if(next_column < 1 || next_column > 6)
    {
      printf("You chose a column that is not on the playable part of the game board. Enter a column number from 1-6.\n");
      move_builder(player, game_board_arr);
    }

    //Determines if the player moved in a straight line and returns a number indicating if they are moving north, south, east, etc....
    direction = player_direction_is_line(player, next_row, next_column, game_board_arr);

    if(direction == 0)
    {
      printf("You chose a new space that is not in a straight line from your last space.\n");
      printf("Please choose a new space.\n");
      move_builder(player, game_board_arr);
    }
    
    is_path_blocked = is_blocked(player, computer_player, next_row, next_column, game_board_arr);

    if(is_path_blocked == 0)
    {
      print_blocking_error();
      move_builder(player, game_board_arr);
    }

    if(direction != 0 && is_path_blocked != 0)
    {
      update_tower_levels(player, direction, next_row, next_column, game_board_arr);
    }
  }
  
  if(next_builder == computer_player)
  {
    int north, south, east, west, computer_column, computer_row, player_row, player_column;
    
    computer_row = get_builder_row_location(computer_player, game_board_arr);
    computer_column = get_builder_column_location(computer_player, game_board_arr);
    
    if(game_board_arr[computer_row][6] != player)
    {
      east = is_blocked(computer_player, player, computer_row, 6, game_board_arr);
    }
    else
    {
      east = is_blocked(computer_player, player, computer_row, 5, game_board_arr);
    }

    if(game_board_arr[computer_row][1] != player)
    {
      west = is_blocked(computer_player, player, computer_row, 1, game_board_arr);
    }
    else
    {
      west = is_blocked(computer_player, player, computer_row, 2, game_board_arr);
    }
    
    if(game_board_arr[1][computer_column] != player)
    {
      north = is_blocked(computer_player, player, 1, computer_column, game_board_arr);
    }
    else
    {
      north = is_blocked(computer_player, player, 2, computer_column, game_board_arr);
    }

    if(game_board_arr[6][computer_column] != player)
    {
      south = is_blocked(computer_player, player, 6, computer_column, game_board_arr);
    }
    else
    {
      south = is_blocked(computer_player, player, 5, computer_column, game_board_arr);
    }
    
    if(north > south && north > east && north > west && game_board_arr[computer_row -1][computer_column] != 0)
    {
      if(game_board_arr[1][computer_column] != player)
      {
        update_tower_levels(computer_player, 3, computer_row - north, computer_column, game_board_arr);
      }
      else
      {
        update_tower_levels(computer_player, 3, computer_row - north, computer_column, game_board_arr);
      }
    }
    
    else if(south >= east && south >= west && south > north && game_board_arr[computer_row+1][computer_column] != 0)
    {
      if(game_board_arr[6][computer_column] != player)
      {
        update_tower_levels(computer_player, 4, computer_row + south, computer_column, game_board_arr);
      }
      else
      {
        update_tower_levels(computer_player, 4, computer_row + south, computer_column, game_board_arr);
      }
    }

    else if(east >= south && east >= north && east >= west && game_board_arr[computer_row][computer_column + 1] != 0)
    {
      if(game_board_arr[computer_row][6] != player)
      {
        update_tower_levels(computer_player, 1, computer_row, computer_column + east, game_board_arr);
      }
      else
      {
        update_tower_levels(computer_player, 1, computer_row, computer_column + east, game_board_arr);
      }
    } 
    
    else if(west >= south && west >= east && west >= north && game_board_arr[computer_row][computer_column - 1] != 0)
    {
      
      update_tower_levels(computer_player, 2, computer_row, computer_column - west, game_board_arr);
    }
    else
    {
      update_tower_levels(computer_player, 2, computer_row, computer_column - west, game_board_arr);
    }
    
  }
}

int is_blocked(char cur_player, char other_player, int next_row, int next_column, int game_board_arr[][7] )
{
  int cur_row, cur_column, player_direction, counter;
  cur_row = get_builder_row_location(cur_player, game_board_arr);
  cur_column = get_builder_column_location(cur_player, game_board_arr);

  player_direction = player_direction_is_line(cur_player, next_row, next_column, game_board_arr);

  if(game_board_arr[next_row][next_column] == other_player)
  {
    return 0;
  }

  counter = 0;
  if(player_direction == 1)
  {
    do{
      cur_column++;

      if(cur_player == player)    
      {
        if(game_board_arr[cur_row][cur_column] == other_player)
        {
            return 0; //Returning 10 if pathwat is obstructed
        }
      }
      
      if(cur_player == computer_player)
      {
        if(game_board_arr[cur_row][cur_column] == other_player || cur_column == next_column)
        {
            return counter + 1;
        }
      }
      counter++;
    }while(cur_column != next_column);

    return 1; //path not blocked
  }
  counter = 0;
  if(player_direction == 2)
  {
    do{
      // printf("\nCur column: %i\n", cur_column);
      // printf("\nNext column: %i\n", next_column);
      cur_column--;
          
      if(cur_player == player)    
      {
        if(game_board_arr[cur_row][cur_column] == other_player)
        {
            return 0; //Returning 10 if pathwat is obstructed
        }
      }

      if(cur_player == computer_player)
      {
        if(game_board_arr[cur_row][cur_column] == player || cur_column == next_column)
        {
            return counter + 1;
        }
      }
      counter++;
      // printf("\nWest counter: %i\n", counter);

    }while(cur_column != next_column);
    
    return 1;
  }
  
  counter = 0;
  if(player_direction == 3)
  {
    do{
      cur_row--;    
      
      if(cur_player == player)    
      {
        if(game_board_arr[cur_row][cur_column] == other_player)
        {
            return 0; //Returning 10 if pathwat is obstructed
        }
      }

      if(cur_player == computer_player)
      {
        if(game_board_arr[cur_row][cur_column] == other_player || cur_row == next_row)
        {
            return counter + 1;
        }
      }
      counter++;

    }while(cur_row != next_row);
    
    return 1;
  }
  
  counter = 0;
  if(player_direction == 4)
  {
    do{
      cur_row++;
          
      if(cur_player == player)    
      {
        if(game_board_arr[cur_row][cur_column] == other_player)
        {
            return 0; //Returning 10 if pathwat is obstructed
        }
      }

      if(cur_player == computer_player)
      {
        if(game_board_arr[cur_row][cur_column] == other_player || cur_row == next_row)
        {
            return counter + 1;
        }
      }
      counter++;

    }while(cur_row != next_row);

    return 1;
  }
  
  if(player_direction == 5)
  {
    do{                                                  
      cur_row--;
      cur_column++;
          
      if(cur_player == player)    
      {
        if(game_board_arr[cur_row][cur_column] == other_player)
        {
            return 0; 
        }
      }

      if(cur_player == computer_player)
      {
        if(game_board_arr[cur_row][cur_column] == other_player || cur_row == next_row)
        {
            return counter + 1;
        }
      }
      counter++;

    }while(cur_column != next_column && cur_row != next_row);

      return 1;
  }
  
  if(player_direction == 6)
  {
    do{                                                  
      cur_row++;
      cur_column--;
      if(cur_player == player)    
      {
        if(game_board_arr[cur_row][cur_column] == other_player)
        {
            return 0; //Returning 10 if pathwat is obstructed
        }
      }

      if(cur_player == computer_player)
      {
        if(game_board_arr[cur_row][cur_column] == other_player || cur_row == next_row)
        {
            return counter + 1;
        }
      }
      counter++;
    }while(cur_column != next_column && cur_row != next_row);

    return 1;
  }
  
  if(player_direction == 7)
  {
    do{                                                  
        cur_row--;
        cur_column--;
                
        if(cur_player == player)    
      {
        if(game_board_arr[cur_row][cur_column] == other_player)
        {
            return 0; //Returning 10 if pathwat is obstructed
        }
      }

      if(cur_player == computer_player)
      {
        if(game_board_arr[cur_row][cur_column] == other_player || cur_row == next_row)
        {
            return counter + 1;
        }
      }
      counter++;
      }while(cur_column != next_column && cur_row != next_row);

      return 1;    
  }
  
  if(player_direction == 8)
  {
    do{                                                  
      cur_row++;
      cur_column++;

      if(cur_player == player)    
      {
        if(game_board_arr[cur_row][cur_column] == other_player)
        {
            return 0; //Returning 10 if pathwat is obstructed
        }
      }

      if(cur_player == computer_player)
      {
        if(game_board_arr[cur_row][cur_column] == other_player || cur_row == next_row)
        {
            return counter + 1;
        }
      }
      counter++;

    }while(cur_column != next_column && cur_row != next_row);
    return 1;
  }
  return 0;
}

int player_direction_is_line(char cur_builder_turn, int next_row, int next_column, int game_board_arr[][7])
{
  int cur_row, cur_column;
  cur_row = get_builder_row_location(cur_builder_turn, game_board_arr);
  cur_column = get_builder_column_location(cur_builder_turn, game_board_arr);


  //Finds difference between current row/column location to determine slope and direction
  int difference_in_row_locations, difference_in_column_locations;
  difference_in_row_locations = cur_row - next_row;
  difference_in_column_locations = cur_column - next_column;

  //Finds remainder of slope (which determines straight line)
  int slope;   
  if(difference_in_row_locations != 0)
  {  
    slope = (difference_in_column_locations / difference_in_row_locations);
  }

  if(difference_in_row_locations == 0 && difference_in_column_locations == 0)
  {
    return 9;
  }

  if(difference_in_row_locations == 0 && difference_in_column_locations != 0)
  {
    if(cur_column < next_column)
    {
      return 1; 
    }
    else
    {
      return 2; 
    }
  }

  if(difference_in_row_locations != 0 && difference_in_column_locations == 0)
  {
    if(cur_row > next_row)
    {
      return 3;
    }
    else
    {
      return 4;
    }
  }

  if(slope == -1 && difference_in_column_locations < 0)
  {
    return 5; //Returning 5 if builder moved in a straight line diagnolly FROM BOTTOM LEFT TO TOP RIGHT & path is clear northeast
  }

  if(slope == -1 && difference_in_column_locations > 0)
  {
    return 6; //Returning 6 if builder moved in a straight line diagnolly FROM TOP RIGHT TO BOTTOM LEFT southwest
  }

  if(slope == 1 && difference_in_column_locations > 0)
  {
    return 7; //Returning 7 if builder moved in a straight line diagnolly FROM BOTTOM RIGHT TO TOP LEFTnorthwest
  }

  if(slope == 1 && difference_in_column_locations < 0)
  {
    return 8; ////Returning 8 if builder moved in a straight line diagnolly FROM TOP LEFT TO BOTTOM RIGHT southeast
  }
  return 0; //Not a straight line
}

void update_tower_levels(char cur_builder_turn, int direction_moved, int next_row, int next_column, int game_board_arr[][7])
{
  int cur_row, cur_column;
  previous_value = game_board_arr[next_row][next_column];
  cur_row = get_builder_row_location(cur_builder_turn, game_board_arr);
  cur_column = get_builder_column_location(cur_builder_turn, game_board_arr);
  
  if(cur_builder_turn == player && previous_value != 4)
  {
    game_board_arr[cur_row][cur_column] = previous_value + 1;
  }
  else
  {
    game_board_arr[cur_row][cur_column] = previous_value;
  }
  
  if(cur_builder_turn == computer_player && previous_value != 0)
  {
    game_board_arr[cur_row][cur_column] = previous_value - 1;
  }
  
  previous_value = game_board_arr[next_row][next_column];  
  
  game_board_arr[next_row][next_column] = cur_builder_turn; 
  
    

  switch(direction_moved)
  {
    case 1:
      do{
        cur_column++;

        previous_value = game_board_arr[cur_row][cur_column];
        
        if(cur_builder_turn == player)
        {
          if(cur_column == next_column)
          {
            game_board_arr[cur_row][cur_column] = previous_value;
          }
          else
          {
            if(game_board_arr[cur_row][cur_column] != 4)
            {
              game_board_arr[cur_row][cur_column] = previous_value + 1;
            }
            else
            {
              game_board_arr[cur_row][cur_column] = previous_value;
            }
          }
        }
        
        if(cur_builder_turn == computer_player)
        {
          if(cur_column == next_column)
          {
            game_board_arr[cur_row][cur_column] = previous_value;
          }
          else
          {
            if(game_board_arr[cur_row][cur_column] != 0)
            {
              game_board_arr[cur_row][cur_column] = previous_value - 1;
            }
            else
            {
              game_board_arr[cur_row][cur_column] = previous_value;
            }
          }
        }
      }while(cur_column != next_column);
      break;
    
    case 2:     
      do{
        cur_column--;
        previous_value = game_board_arr[cur_row][cur_column];
        
        if(cur_builder_turn == player)
        {
          if(cur_column == next_column)
          {
            game_board_arr[cur_row][cur_column] = previous_value;
          }
          else
          {
            if(game_board_arr[cur_row][cur_column] != 4)
            {
              game_board_arr[cur_row][cur_column] = previous_value + 1;
            }
            else
            {
              game_board_arr[cur_row][cur_column] = previous_value;
            }
          }
        }
        if(cur_builder_turn == computer_player)
        {
          if(cur_column == next_column)
          {
            game_board_arr[cur_row][cur_column] = previous_value;
          }
          else
          {
            if(game_board_arr[cur_row][cur_column] != 0)
            {
              game_board_arr[cur_row][cur_column] = previous_value - 1;
            }
            else
            {
              game_board_arr[cur_row][cur_column] = previous_value;
            }
          }
        }
      }while(cur_column != next_column);
      break;

    case 3:
      do{
        cur_row--;
        previous_value = game_board_arr[cur_row][cur_column];
        
        if(cur_builder_turn == player)
        {
          if(cur_row == next_row)
          {
            game_board_arr[cur_row][cur_column] = previous_value;
          }
          else
          {
            if(game_board_arr[cur_row][cur_column] != 4)
            {
              game_board_arr[cur_row][cur_column] = previous_value + 1;
            }
            else
            {
              game_board_arr[cur_row][cur_column] = previous_value;
            }
          }
        }
        
        if(cur_builder_turn == computer_player)
        {
          if(cur_row == next_row)
          {
            game_board_arr[cur_row][cur_column] = previous_value;
          }
          else
          {
            if(game_board_arr[cur_row][cur_column] != 0)
            {
              game_board_arr[cur_row][cur_column] = previous_value - 1;
            }
            else
            {
              if(game_board_arr[cur_row][cur_column] != 0)
              {
                game_board_arr[cur_row][cur_column] = previous_value - 1;
              }
              else
              {
                game_board_arr[cur_row][cur_column] = previous_value;
              }
            }
          }
        }
      }while(cur_row != next_row);
      break;

    case 4:
        do{
          cur_row++;
          
          previous_value = game_board_arr[cur_row][cur_column];
          
          if(cur_builder_turn == player)
          {
            if(cur_row == next_row)
            {
              game_board_arr[cur_row][cur_column] = previous_value;
            }
            else
            {
              if(game_board_arr[cur_row][cur_column] != 4)
              {
                game_board_arr[cur_row][cur_column] = previous_value + 1;
              }
              else
              {
                game_board_arr[cur_row][cur_column] = previous_value;
              }
            }
          }       
          if(cur_builder_turn == computer_player)
          {
            if(cur_row == next_row)
            {
              game_board_arr[cur_row][cur_column] = previous_value;
            }
            else
            {
              if(game_board_arr[cur_row][cur_column] != 0)
              {
                game_board_arr[cur_row][cur_column] = previous_value - 1;
              }
              else
              {
                game_board_arr[cur_row][cur_column] = previous_value;
              }
            }
          }
        }while(cur_row != next_row);
        break;

    case 5:
        do{                                                  
          cur_row--;
          cur_column++;
          previous_value = game_board_arr[cur_row][cur_column];
          
          if(cur_builder_turn == player)
          {
            if(cur_column == next_column && cur_row == next_row)
            {
              game_board_arr[cur_row][cur_column] = previous_value;
            }
            else
            {
              if(game_board_arr[cur_row][cur_column] != 4)
              {
                game_board_arr[cur_row][cur_column] = previous_value + 1;
              }
              else
              {
                game_board_arr[cur_row][cur_column] = previous_value;
              }
            }
          }       
          if(cur_builder_turn == computer_player)
          {
            if(cur_row == next_row)
            {
              game_board_arr[cur_row][cur_column] = previous_value;
            }
            else
            {
              if(game_board_arr[cur_row][cur_column] != 0)
              {
                game_board_arr[cur_row][cur_column] = previous_value - 1;
              }
              else
              {
                game_board_arr[cur_row][cur_column] = previous_value;
              }
            }
          }                   
        }while(cur_column != next_column && cur_row != next_row);
        break;
          
    case 6:
        do{                                                  
          cur_row++;
          cur_column--;
          previous_value = game_board_arr[cur_row][cur_column];

          if(cur_builder_turn == player)
          {
            if(cur_column == next_column)
            {
              game_board_arr[cur_row][cur_column] = previous_value;
            }
            else
            {
              if(game_board_arr[cur_row][cur_column] != 4)
              {
                game_board_arr[cur_row][cur_column] = previous_value + 1;
              }
              else
              {
                game_board_arr[cur_row][cur_column] = previous_value;
              }
            }
          }       
          if(cur_builder_turn == computer_player)
          {
            if(cur_row == next_row)
            {
              game_board_arr[cur_row][cur_column] = previous_value;
            }
            else
            {
              if(game_board_arr[cur_row][cur_column] != 0)
              {
                game_board_arr[cur_row][cur_column] = previous_value - 1;
              }
              else
              {
                game_board_arr[cur_row][cur_column] = previous_value;
              }
            }
          }                  
        }while(cur_column != next_column && cur_row != next_row);
        break;

    case 7: 
        do{                                                  
          cur_row--;
          cur_column--;
          previous_value = game_board_arr[cur_row][cur_column];

          if(cur_builder_turn == player)
          {
            if(cur_column == next_column)
            {
              game_board_arr[cur_row][cur_column] = previous_value;
            }
            else
            {
              if(game_board_arr[cur_row][cur_column] != 4)
              {
                game_board_arr[cur_row][cur_column] = previous_value + 1;
              }
              else
              {
                game_board_arr[cur_row][cur_column] = previous_value;
              }
            }
          }       
          if(cur_builder_turn == computer_player)
          {
            if(cur_row == next_row)
            {
              game_board_arr[cur_row][cur_column] = previous_value;
            }
            else
            {
              if(game_board_arr[cur_row][cur_column] != 0)
              {
                game_board_arr[cur_row][cur_column] = previous_value - 1;
              }
              else
              {
                game_board_arr[cur_row][cur_column] = previous_value;
              }
            }
          }                   
        }while(cur_column != next_column && cur_row != next_row);
        break;

    case 8:
        do{                                                  
          cur_row++;
          cur_column++;
          previous_value = game_board_arr[cur_row][cur_column];

          if(cur_builder_turn == player)
          {
            if(cur_column == next_column)
            {
              game_board_arr[cur_row][cur_column] = previous_value;
            }
            else
            {
              if(game_board_arr[cur_row][cur_column] != 4)
              {
                game_board_arr[cur_row][cur_column] = previous_value + 1;
              }
              else
              {
                game_board_arr[cur_row][cur_column] = previous_value;
              }
            }
          }       
          if(cur_builder_turn == computer_player)
          {
            if(cur_row == next_row)
            {
              game_board_arr[cur_row][cur_column] = previous_value;
            }
            else
            {
              if(game_board_arr[cur_row][cur_column] != 0)
              {
                game_board_arr[cur_row][cur_column] = previous_value - 1;
              }
              else
              {
                game_board_arr[cur_row][cur_column] = previous_value;
              }
            }
          }                         
        }while(cur_column != next_column && cur_row != next_row);
          break;

    default:
        break;
  }
}

int check_game_over(int game_board_arr[][7])
{
  int num_fours, num_zeros;
  int four = 4;
  int zero = 0;
  

  num_fours = traverse_board(four, game_board_arr);
  num_zeros = traverse_board(zero, game_board_arr);
  int x = traverse_board(4, game_board_arr);

  if(num_fours >= 10)
  {
    return 1; //User wins
  }
  if(num_zeros >= 10)
  {
    return 2; //CPU wins
  }
  return 0; //game still going 
}

int traverse_board(int search, int game_board_arr[][7])
{
  int counter = 0;
  

  for(int i = 1; i < 7; i++)
  {
      for(int j = 1; j < 7; j++)
      {
        int x = game_board_arr[i][j];
        if(game_board_arr[i][j] == search)
        {
          counter++;  
        }
      }
  }
  return counter;
}

int main(void) 
{
  int is_over;
  
  printf("Welcome to the game Santorini! This is the starting game board: \n\n");
  int game_board[][7] = 
  {
    ' ', 1, 2, 3, 4, 5, 6,
    1, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2,
    3, 2, 2, 2, 2, 2, 2,
    4, 2, 2, 2, 2, 2, 2,
    5, 2, 2, 2, 2, 2, 2,
    6, 2, 2, 2, 2, 2, 2,
  };

  print_board(game_board);
 
  starting_spot(game_board); //2nd step
  
  
  do{
    move_builder(player, game_board);
    int x = traverse_board(4, game_board);
    is_over = check_game_over(game_board);

    move_builder(computer_player, game_board);
    is_over = check_game_over(game_board);

    if(is_over == 1)
    {
      printf("\nThere are 10 four story buildings in the game!\n");
      printf("You win!\n");
    }
    if(is_over == 2)
    {
      printf("\nThere are 10 zero story buildings in the game!\n");
      printf("You Lose :(\n");
    }

  }while(is_over == 0);

  printf("Game Over");

  return 0;
}

