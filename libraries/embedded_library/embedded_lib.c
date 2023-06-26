/**
 * Function to generate a random number within a range
 */
int getRandomNumber(int min, int max)
{
  return min + rand() % (max - min + 1);
}

/**
 * Function to clear the serial monitor by printing new lines
 */
void clearSerialMonitor()
{
  for (int i = 0; i < 50; i++)
    printf("\n");
}