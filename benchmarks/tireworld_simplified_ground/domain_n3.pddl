(define (domain triangle-tire-n3)
  (:requirements :typing :strips :non-deterministic)
  (:types location)
  (:constants l1 l2 l3 - location)
  (:predicates
    (at ?loc - location)
    (spare-in ?loc - location)
    (not-flattire)
  )
  (:action move_l1_l2
    :precondition (and (at l1) (not-flattire))
    :effect (and
      (at l2)
      (not (at l1))
      (oneof (and) (not (not-flattire)))
    )
  )
  (:action move_l2_l1
    :precondition (and (at l2) (not-flattire))
    :effect (and
      (at l1)
      (not (at l2))
      (oneof (and) (not (not-flattire)))
    )
  )
  (:action move_l2_l3
    :precondition (and (at l2) (not-flattire))
    :effect (and
      (at l3)
      (not (at l2))
      (oneof (and) (not (not-flattire)))
    )
  )
  (:action move_l3_l2
    :precondition (and (at l3) (not-flattire))
    :effect (and
      (at l2)
      (not (at l3))
      (oneof (and) (not (not-flattire)))
    )
  )
  (:action changetire
    :parameters (?loc - location)
    :precondition (and (spare-in ?loc) (at ?loc))
    :effect (and (not (spare-in ?loc)) (not-flattire))
  )
)
