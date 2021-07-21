SELECT name FROM people JOIN
	(SELECT stars.person_id AS pid, stars.movie_id AS mid FROM stars JOIN
	movies ON stars.movie_id == movies.id 
	WHERE movies.year == 2004 )
ON pid == people.id 
GROUP by pid
ORDER by birth;