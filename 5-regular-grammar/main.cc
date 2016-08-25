/*******************************************************************************
 * Author: Dyinnz.HUST.UniqueStudio
 * Email:  ml_143@sina.com
 * Github: https://github.com/dyinnz
 * Date:   2016-08-25
 ******************************************************************************/

#include <iostream>
#include "simplelogger.h"

using namespace std;
using namespace simple_logger;

BaseLogger logger;

int main() {
  cout << "hello world" << endl;
  logger.log("this is logger");
  return 0;
}
