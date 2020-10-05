#include <string>
#include <iostream>
#include <fstream>

#include "help.h"
#include "ebook.h"


using namespace std;

int *find_position(const char text[], const char pattern[])
{
  static int index[MAX_LEN];
  index[0] = -1;
  int times = 0 , last_find = 0;
  string const s = text;
  while (true)
  {
    last_find = s.find(pattern, times + last_find);
    if (last_find != string::npos)
    {
      index[times] = last_find;
      times++;
    }
    else
	{
	  break;
    }
  }
  times++;
  index[times] = -1;
  return index;
}

bool RunCmd(EBook &book, int &pid, const char cmd[])
{
  static int saved_page;
  if (strcmp(cmd, "Search") == 0)
  {
    FindString(book, pid);
    return true;
  }
  else if (strncmp(cmd, "goto ", 5) == 0)
  {
    const char* tmp = &cmd[5];
    istringstream iss;
    iss.str(tmp);
    iss >> pid;
    if (book.if_start)
    {
      if (!book.page[pid].if_go)
      {
        book.total_score += book.page[pid].score;
        book.page[pid].if_go = true;
      }
      book.step_count++;
    }
    if (pid == 23)
    {
      book.if_read = false;
      book.if_start = false;
    }
    
    PrintPage(book, pid);
    return true;
  }
  else if (strcmp(cmd, "Qsave") == 0)
  {
    saved_page = pid;
    return true;
  }
  else if (strcmp(cmd , "Qload") == 0)
  {
    if (DoubleConfirmation(book, pid, cmd))
    {
      book.if_start = false;
      pid = saved_page;
      PrintPage(book , pid);
    }
    else
    {
      PrintCmd(book.page[pid], book);
    }
        
    return true;
  }
  else if (strcmp(cmd, "exit") == 0)
  {
    if (pid == 23)
    {
      book.if_end = true;
      return false;
    }
    
    if (DoubleConfirmation(book, pid, cmd))
    {
      book.if_read = false;
      return false;
    }
    else
    {
      PrintCmd(book.page[pid], book);
      return true;
    }
        
  }
  else if (strcmp(cmd, "start") == 0)
  {
    book.total_score = 0;
    pid = 24;
    book.if_start = true;
    for (int i = 0; i < 23; i++)
    {
      book.page[i].if_go = false;
    }
    
    PrintPage(book, pid);
    return true;
  }
  else if (strcmp(cmd, "Check score"))
  {
    CheckScore(book, pid);
    return true;
  }
    
  return false;
}

int main()
{
  EBook book;
  book.if_read = true;
  const char *bookname = "squirrel_survival.txt";  /*book's name*/
  if (!LoadBook(bookname, book))
  {
    return -1;
  }

  //book start
  CleanScreen();
  cout << "load book " << bookname << "!" << endl;
  cout << "Press any key to read the book" << endl;
  Getch();

  int current = 0;
  PrintPage(book, current);
  while (true)
  {
    if (!GetCmd(book, current))
    {
      if (book.if_read)
      {
        cerr << "Something went wrong..." << endl;
        break;
      }
      else
      {
        CleanScreen();
        if (book.if_end)
        {
          int a = 23;
          CheckScore(book, a);
        }
        //to say when exit
        cerr << "Thanks for reading." << endl;
        break;
      }
    }
  }

  return 0;
}
