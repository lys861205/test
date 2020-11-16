#include <string>

using namespace std;

class GuessStatisticsMessage 
{
private:
  string number;
  string verb;
  string pluralModifier;
public:
  string Make(char candidate, int count)
  {
    createPluralDependentMessageParts(count);
    char buf[128] = {0};
    snprintf(buf, sizeof buf, "There %s %s %c%s", verb.c_str(), number.c_str(), candidate, pluralModifier.c_str());
    return std::move(string(buf));
  }

  void thereAreNoLetters() {
  }
  void thereIsOneLetter() {
  }
  void thereAreManyLetters(int count) {
  }

  void createPluralDependentMessageParts(int count)
  {
    if (count == 0) {
      thereAreNoLetters();
    } else if (count == 1) {
      thereIsOneLetter();
    } else {
      thereAreManyLetters(count);
    }
  }
};

int main()
{
  return 0;
}
