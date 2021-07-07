#include <stdio.h>
#include <cs50.h>

// Prototypes
int get_number_len(long number);

const long MAX_CARD_NUMBER = 999999999999999999;
const long MIN_CARD_NUMBER = 1000000000000;

const long DECIM_13_MIN = 1000000000000;
const long DECIM_13_MAX = 9999999999999;
const long DECIM_15_MIN = 100000000000000;
const long DECIM_15_MAX = 999999999999999;
const long DECIM_16_MIN = 1000000000000000;
const long DECIM_16_MAX = 9999999999999999;


// **********************************
// enter card number
// **********************************
long get_card_number(void)
{
    long card_number;

    do
    {
        card_number = get_long("Number: ");

    } while (card_number < 0 );

    return card_number;
}

// ************************************************
// summarize the multiplied 2 every other digits
// ************************************************
int get_sum_every_other(long number)
{
    int card_number_length = get_number_len(number);
    int sum = 0;

    for (long i = card_number_length - 1, j = 10; i >=0; i--, j *= 100)
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

// **********************************
// sum every first digits
// **********************************
int get_sum_every_first(long number)
{
    int sum = 0;

    for (long i = 0, j = 1; i < 8; i++, j *= 100)
    {
        sum += (number / j) % 10;
    }

    return sum;
}

// **********************************
// check if card valid
// **********************************
bool is_card_valid(long card_number)
{

    // check numbers quantity
    if (card_number > MAX_CARD_NUMBER || card_number < MIN_CARD_NUMBER) return false;

   // check if card is valid (by Luhn algorithm)
    int sum_every_other = get_sum_every_other(card_number);
    printf("%i ", sum_every_other);

    int sum_every_first = get_sum_every_first(card_number);
    printf("%i\n", sum_every_first);

    int sum = sum_every_other + sum_every_first;

    if (sum % 10 == 0) return true;
    else return false;

}

// **********************************
// calculate length of card number
// **********************************
int get_number_len(long number)
{
    if (number >= DECIM_13_MIN && number <= DECIM_13_MAX)      return 13;
    else if (number >= DECIM_15_MIN && number <= DECIM_15_MAX) return 15;
    else if (number >= DECIM_16_MIN && number <= DECIM_16_MAX) return 16;
    else return 0;
}

// **************************************
// extract first 2 digits from card number
// ***************************************
long get_first_two_digit(long number)
{
    int number_length =  get_number_len(number);

    // not valid card namber length
    if (number_length == 0) return 0;

    int first_two_digit;

    // calculate first 2 digits
    if (number_length == 13) first_two_digit = (number / (DECIM_13_MIN / 10));
    if (number_length == 15) first_two_digit = (number / (DECIM_15_MIN / 10));
    if (number_length == 16) first_two_digit = (number / (DECIM_16_MIN / 10));

    return first_two_digit;
}

// *************************************
// return car type based on firs 2 digits
// *************************************
string get_card_type(int first_two_digit)
{
    if (first_two_digit == 34 || first_two_digit == 37) return "AMEX";
    if (first_two_digit >= 51 && first_two_digit <= 55) return "MASTERCARD";
    if (first_two_digit >= 40 && first_two_digit <= 49) return "VISA";

    return "INVALID";
}

int main(void)
{
    // get card number from input
    long card_number = get_card_number();

    // get 2 fist digits of the card number
    int first_two_digit = get_first_two_digit(card_number);
    printf("%i\n", first_two_digit);

    // get card type
    string card_type = get_card_type(first_two_digit);
    printf("%s\n", card_type);

    // check if card is valid by Luhns algotrthm
    if (is_card_valid(card_number)) printf("%s\n", card_type);
    else printf ("%s\n", "INVALID");

}
