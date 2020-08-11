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
    return string.format("There %s %s %s%s", verb, number, candidate, pluralModifier);
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
