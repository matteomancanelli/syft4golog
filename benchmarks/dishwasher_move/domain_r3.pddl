(define (domain dishwasher)
  (:requirements :typing :strips :non-deterministic)
  (:types room dish)
  (:constants r0 - room)

  (:predicates
    (robot-at ?r - room)
    (can-arrive ?d - dish)
    (dirty-dish-at ?d - dish ?r - room)
    (clean ?d - dish)
    (holding ?d - dish)
    (emptyhand)
  )

  (:action move
    :parameters (?from - room ?to - room)
    :precondition (and (robot-at ?from))
    :effect (and (robot-at ?to) (not (robot-at ?from)))
  )

  (:action ask
    :parameters (?d - dish)
    :precondition (and (robot-at r0) (can-arrive ?d))
    :effect (oneof
        (and (dirty-dish-at ?d r1) (not (can-arrive ?d)))
        (and (dirty-dish-at ?d r2) (not (can-arrive ?d)))
        (and (dirty-dish-at ?d r3) (not (can-arrive ?d)))
      )
  )

  (:action load
    :parameters (?r - room ?d - dish)
    :precondition (and
      (robot-at ?r)
      (dirty-dish-at ?d ?r)
      (emptyhand)
    )
    :effect (and 
      (not (emptyhand)) 
      (not (dirty-dish-at ?d ?r)) 
      (holding ?d)
    )
  )

  (:action unload
    :parameters (?d - dish)
    :precondition (and
      (robot-at r0)
      (holding ?d)
    )
    :effect (and
      (emptyhand)
      (not (holding ?d))
      (clean ?d)
    )
  )
)
