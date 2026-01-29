(define (domain warehouse-domain)
	(:requirements :non-deterministic :typing)
	(:types box shelf)
	(:predicates (robot-at ?l - shelf)(box-at ?b - box ?l - shelf)(broken ?b - box)(holding ?b - box)(emptyhand)(has-wrap ?b - box))
	(:action move
	:parameters (?l1 - shelf ?l2 - shelf)
	:precondition (and (robot-at ?l1))
	:effect (and (not (robot-at ?l1)) (robot-at ?l2))
	)
	(:action take
	:parameters (?b - box ?l - shelf)
	:precondition (and (robot-at ?l)(box-at ?b ?l)(emptyhand))
	:effect (and (holding ?b)(not (emptyhand))(not (box-at ?b ?l)))
	)
	(:action wrap
	:parameters (?b - box)
	:precondition (and (holding ?b))
	:effect (and (has-wrap ?b))
	)
	(:action drop
	:parameters (?b - box ?l - shelf)
	:precondition (and (robot-at ?l) (holding ?b))
	:effect (oneof
		(and (box-at ?b ?l) (emptyhand) (not (holding ?b)))
		(and (broken ?b) (emptyhand) (not (holding ?b)))))
	(:action drop-wrap
	:parameters (?b - box ?l - shelf)
	:precondition (and (robot-at ?l) (holding ?b) (has-wrap ?b))
	:effect (and (box-at ?b ?l) (emptyhand) (not (holding ?b))))
)
