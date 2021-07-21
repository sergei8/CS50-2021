SELECT people.name FROM people JOIN
	(SELECT directors.person_id as pid FROM directors JOIN
		(SELECT movies.id AS mid FROM ratings JOIN movies 
		ON ratings.movie_id == movies.id 
		WHERE ratings.rating >= 9.0)
	ON directors.movie_id == mid)
ON pid == people.id
GROUP BY pid;