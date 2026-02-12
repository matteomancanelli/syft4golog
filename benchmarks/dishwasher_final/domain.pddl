(define (domain warehouse-domain)
	(:requirements :non-deterministic :typing)
	(:types dish room)
	(:constants r0 - room)
	(:predicates (robot-at ?r - room)(dish-at ?d - dish ?r - room)(dirty ?d - dish)(holding ?d - dish)(emptyhand)(been_washed ?d - dish))
	(:action move
	:parameters (?r1 - room ?r2 - room)
	:precondition (and (robot-at ?r1))
	:effect (and (not (robot-at ?r1)) (robot-at ?r2))
	)
	(:action load
	:parameters (?d - dish ?r - room)
	:precondition (and (robot-at ?r)(dish-at ?d ?r)(emptyhand))
	:effect (and (holding ?d)(not (emptyhand))(not (dish-at ?d ?r)))
	)
	(:action wash
	:parameters (?d - dish)
	:precondition (and (dirty ?d))
	:effect (and (holding ?d) (not (emptyhand)) (not (dirty ?d)) (been_washed ?d))
	)
	(:action try_unload
	:parameters (?d - dish ?r - room)
	:precondition (and (robot-at ?r) (holding ?d) (not (been_washed ?d)))
	:effect (oneof
		(and (not (dirty ?d)) (dish-at ?d ?r) (emptyhand) (not (holding ?d)))
		(and (dirty ?d) (not (been_washed ?d)))))
	(:action unload_clean
	:parameters (?d - dish ?r - room)
	:precondition (and (robot-at ?r) (holding ?d) (been_washed ?d))
	:effect (and (dish-at ?d ?r) (emptyhand) (not (holding ?d))))
)
