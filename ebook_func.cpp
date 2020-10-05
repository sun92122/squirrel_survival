#include <string>
#include <iostream>
#include <fstream>

#include "ebook.h"
void CleanScreen()
{
#ifdef _WIN32
  system("cls");
#else
  system("clear");
#endif
}
int text_line = 0, cmd_line = 0;
void CleanText(Page &page)
{
  for (int i = 0; i < cmd_line + text_line; ++i)
  {
    std::cout << PLINE;
    for (int i = 0; i < 100; ++i)
    {
      std::cout << '\b';
	}
    for (int i = 0; i < 100; ++i)
    {
      std::cout << ' ';
	}
    for (int i = 0; i < 100; ++i)
    {
      std::cout << '\b';
	}
  }
  text_line = cmd_line = 0;
}

void CleanCmd(Page &page)
{
  for (int i = 0; i < cmd_line; ++i)
  {
    std::cout << PLINE;
    for (int i = 0; i < 100; ++i)
    {
      std::cout << '\b';
    }
    for (int i = 0; i < 100; ++i)
    {
      std::cout << ' ';
    }
    for (int i = 0; i < 100; ++i)
    {
      std::cout << '\b';
    }
  }
  cmd_line = 0;
}

void PrintCmd(Page &page, EBook &book)
{
  cmd_line = 1;
  std::cout << "Please enter command: " << std::endl;
  if (!book.if_start)
  {
    page.cmd['h'] = "start";
    std::cout << "[h]: start" << std::endl;
    ++cmd_line;
  }
  for (int i = 0; i < 256 ; ++i)
  {
    if (i == 'h') continue;
    if (page.cmd[i] != "")
    {
      std::cout << "[" << char(i) << "]: " << page.cmd[i] << std::endl;
      ++cmd_line;
    }
  }
}

void CheckScore(EBook &book,int &pid)
{
  CleanCmd(book.page[pid]);
  if (!book.if_start && pid != 23)
  {
    std::cout << "You aren't playing, enter [h] to start or";
  }
  else
  {
    std::cout << "Your score is: " << book.total_score << std::endl;
    std::cout << "Your total step is: " << book.step_count << std::endl;
    cmd_line += 2;
  }
  if (!book.if_end)
  {
    std::cout << "enter [r] to return." << std::endl;
    ++cmd_line;
    while (int c = Getch())
    {
      if (c == 'r')
      {
        CleanCmd(book.page[pid]);
        PrintCmd(book.page[pid], book);
        return;
      }
      if (c == 'h' && !book.if_start && pid != 23)
      {
        RunCmd(book, pid, "start");
        return;
      }
    }
  }
  
}

bool DoubleConfirmation(EBook &book, int &pid, const char cmd[])
{
  if (strcmp(cmd, "Qload") == 0)
  {
    CleanCmd(book.page[pid]);
    std::cout << "The game will stop after loading" <<std::endl;
    std::cout << "Are you sure to load?  y/n" << std::endl;
    cmd_line += 2;
  }
  if (strcmp(cmd, "exit") == 0)
  {
    CleanCmd(book.page[pid]);
    std::cout << "Are you sure to exit?  y/n" << std::endl;
    cmd_line++;
  }
    
  while(int c = Getch())
  {
    if (c == 'y')
    {
      return true;
    }
    if (c == 'n')
    {
      CleanCmd(book.page[pid]);
      return false;
    }
        
  }  
}

void PrintText(Page &page, int pid, EBook &book, bool animation, int *indices, int p_len)
{
  int len = strlen(page.text), cur = 0;
  text_line = 0;
  for (int i = 0, char_count = 0; i < len; ++i)
  {
    if (animation)
    SleepMs(50);
    if (page.text[i] == '\n' || (page.text[i] == ' ' && char_count >= 60))
    {
      std::cout << std::endl;
      ++text_line;
      char_count = 0;
    }
    else
    {
      if (p_len && i == indices[cur])
      {
        std::cout << GREEN;
	  }
      std::cout << page.text[i] << std::flush;
      if (p_len && i == indices[cur] + p_len - 1)
      {
        std::cout << RESET;
        ++cur;
      }
      ++char_count;
    }
  }
  std::cout << std::string(100, '=') << std::endl;
  std::cout << "page " << pid << ".\n"
            << std::endl;
  text_line += 3;
  PrintCmd(page, book);
}

void PrintPage(EBook &book, int pid)
{
  CleanScreen();
  std::cout << book.page[pid].image << std::endl;
  std::cout << std::string(100, '=') << std::endl;
  PrintText(book.page[pid], pid, book);
}

void FindString(EBook &book, int& pid) 
{
  CleanCmd(book.page[pid]);
  std::cout << "Please enter the string to look for: " << std::flush;
  ++cmd_line;
  std::string pattern;
  std::cin >> pattern;
  int *indices = find_position(book.page[pid].text, pattern.c_str());
  CleanText(book.page[pid]);
  PrintText(book.page[pid], pid, book, false, indices, pattern.length());
}

int GetCmd(EBook &book, int &pid)
{
  while (int c = Getch())
  {
    if (book.page[pid].cmd[c] != "")
    {
      return RunCmd(book, pid, book.page[pid].cmd[c].c_str());
    }
  }
  return -1;
}
