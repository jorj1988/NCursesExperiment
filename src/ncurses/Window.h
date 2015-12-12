#pragma once
#include "ncurses/Position.h"
#include "ncurses/Painter.h"

#include <cstddef>
#include <vector>
#include <ncurses.h>
#include <panel.h>

namespace ncurses
{

/// Window containing all ncurses interactions
class Window : public Painter
{
public:
  Window(WINDOW *w, bool delete_window)
  {
    _screen = w;
    delete_window = delete_window;

    box(_screen, '1', '2');

    clear();
  }

  ~Window()
  {
    if (_delete_window)
    {
      delwin(_screen);
    }
  }

  Size size()
  {
    int x, y;
    getmaxyx(_screen, y, x);
    return { x, y };
  }

  void resize(Size s)
  {
    wresize(_screen, s.y(), s.x());
  }

  Position position()
  {
    int x, y;
    getbegyx(_screen, y, x);
    return { x, y };
  }

  void move_to(Position p)
  {
    mvwin(_screen, p.y(), p.x());
  }

  void clear()
  {
    ::wclear(_screen);
  }

  using Painter::print;
  void print(Position pos, const char *data, std::size_t len) override
  {
    mvwaddnstr(_screen, pos.y(), pos.x(), data, len);
  }

  void push_refresh_block()
  {
    ++refresh_block_count;
  }

  void pop_refresh_block()
  {
    --refresh_block_count;
    refresh();
  }

  void refresh()
  {
    if (refresh_block_count == 0)
    {
      ::wrefresh(_screen);
    }
  }

protected:
  WINDOW *_screen = nullptr;
  bool _delete_window = false;
  std::size_t refresh_block_count = 0;

  friend class Context;
};

}