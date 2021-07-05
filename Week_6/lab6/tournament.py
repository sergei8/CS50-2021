# Simulate a sports tournament

import csv
import sys
import random

# Number of simluations to run
N = 1000


def main():

    # Ensure correct usage
    if len(sys.argv) != 2:
        sys.exit("Usage: python tournament.py FILENAME")

    teams = []
    # Read teams into memory from file
    file_name = sys.argv[1]

    # get list of dict  from file `file_name`
    teams_list = list(csv.DictReader(open(file_name)))

    # convert `ratings` to int
    teams_list = list(map(lambda x: {"team": x["team"], "rating": int(x["rating"])}, teams_list))

    # check for pairs
    if len(teams_list) % 2 != 0:
        sys.exit(f"Can't find pair teams in {len(teams_list)}")

    counts = {}
    # Simulate N tournaments and keep track of win counts
    for _ in range(N):
        winner = simulate_tournament(teams_list)
        counts[winner] = counts[winner] + 1 if winner in counts else 1

    # Print each team's chances of winning, according to simulation
    for team in sorted(counts, key=lambda team: counts[team], reverse=True):
        print(f"{team}: {counts[team] * 100 / N:.1f}% chance of winning")


def simulate_game(team1, team2):
    """Simulate a game. Return True if team1 wins, False otherwise."""
    rating1 = team1["rating"]
    rating2 = team2["rating"]
    probability = 1 / (1 + 10 ** ((rating2 - rating1) / 600))
    return random.random() < probability


def simulate_round(teams):
    """Simulate a round. Return a list of winning teams."""
    winners = []

    # Simulate games for all pairs of teams
    for i in range(0, len(teams), 2):
        if simulate_game(teams[i], teams[i + 1]):
            winners.append(teams[i])
        else:
            winners.append(teams[i + 1])

    return winners


def simulate_tournament(teams):
    """Simulate a tournament. Return name of winning team."""

    while (len(teams) > 1):
        teams = simulate_round(teams)

    return teams[0]["team"]


if __name__ == "__main__":
    main()
