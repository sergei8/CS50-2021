SELECT movies.title FROM movies 
JOIN
	(SELECT * FROM
		(SELECT movie_id as mid, count(movie_id) AS cnt FROM stars JOIN people ON stars.person_id == people.id
		WHERE people.name == "Johnny Depp" OR people.name== "Helena Bonham Carter"
		GROUP BY movie_id)
	WHERE cnt == 2)
ON
	movies.id == mid