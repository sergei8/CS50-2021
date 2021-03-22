#include <stdio.h>
#include <cs50.h>


long MAX_CARD_NUMBER = 9999999999999999;
long MIN_CARD_NUMBER = 1000000000000000;

// enter card number
long get_card_number(void)
{
    long card_number;

    do
    {
        card_number = get_long("Number: ");

    } while (card_number < 0 );

    return card_number;
}

// calculate sum of multiplied 2 every other digits
int get_sum_every_other(long number)
{
    int sum = 0;

    for (long i = 0, j = 10; i < 8; i++, j *= 100)
    {

        int digit = ((number / j) % 10) * 2;

        // check if product has 2 digits and devide it for two digits
        if (digit < 10)
        {
            sum += digit;
        }
        else
        {
            int last_digit = digit % 10;
            int first_digit = 1;
            sum += (first_digit + last_digit);
        }

    }

    return sum;
}

// sum every first digits
int get_sum_every_first(long number)
{
    int sum = 0;

    for (long i = 0, j = 1; i < 8; i++, j *= 100)
    {
        sum += (number / j) % 10;
    }

    return sum;
}

// check if card valid
bool is_card_valid(long card_number)
{

    // check numbers quantity
    if (card_number > MAX_CARD_NUMBER || card_number < MIN_CARD_NUMBER) return false;

    // check if card is valid (by Luhn algorithm)
    int sum_every_other = get_sum_every_other(card_number);
    // printf("%i ", sum_every_other);

    int sum_every_first = get_sum_every_first(card_number);
    // printf("%i\n", sum_every_first);

    int sum = sum_every_other + sum_every_first;

    if (sum % 10 == 0) return true;
    else return false;

}

int main(void)
{
    // get card number from input
    long card_number = get_card_number();

    // check if card is valid
    if (is_card_valid(card_number)) printf("VISA\n");
    else printf("INVALID\n");

}