#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <vector>

class SH101;
class View;

class Controller
{
public:
  Controller(SH101 * sh101, const View * view);
  void processEvents();
  bool running;

private:
  SH101 * sh101;
  const View * view;
  int selectedComponent;
};

#endif //CONTROLLER_H
