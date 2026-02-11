(define (domain triangle-tire-n8)
  (:requirements :typing :strips :non-deterministic)
  (:types location)
  (:constants l1 l2 l3 l4 l5 l6 l7 l8 - location)
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
  (:action move_l3_l4
    :precondition (and (at l3) (not-flattire))
    :effect (and
      (at l4)
      (not (at l3))
      (oneof (and) (not (not-flattire)))
    )
  )
  (:action move_l4_l3
    :precondition (and (at l4) (not-flattire))
    :effect (and
      (at l3)
      (not (at l4))
      (oneof (and) (not (not-flattire)))
    )
  )
  (:action move_l4_l5
    :precondition (and (at l4) (not-flattire))
    :effect (and
      (at l5)
      (not (at l4))
      (oneof (and) (not (not-flattire)))
    )
  )
  (:action move_l5_l4
    :precondition (and (at l5) (not-flattire))
    :effect (and
      (at l4)
      (not (at l5))
      (oneof (and) (not (not-flattire)))
    )
  )
  (:action move_l5_l6
    :precondition (and (at l5) (not-flattire))
    :effect (and
      (at l6)
      (not (at l5))
      (oneof (and) (not (not-flattire)))
    )
  )
  (:action move_l6_l5
    :precondition (and (at l6) (not-flattire))
    :effect (and
      (at l5)
      (not (at l6))
      (oneof (and) (not (not-flattire)))
    )
  )
  (:action move_l6_l7
    :precondition (and (at l6) (not-flattire))
    :effect (and
      (at l7)
      (not (at l6))
      (oneof (and) (not (not-flattire)))
    )
  )
  (:action move_l7_l6
    :precondition (and (at l7) (not-flattire))
    :effect (and
      (at l6)
      (not (at l7))
      (oneof (and) (not (not-flattire)))
    )
  )
  (:action move_l7_l8
    :precondition (and (at l7) (not-flattire))
    :effect (and
      (at l8)
      (not (at l7))
      (oneof (and) (not (not-flattire)))
    )
  )
  (:action move_l8_l7
    :precondition (and (at l8) (not-flattire))
    :effect (and
      (at l7)
      (not (at l8))
      (oneof (and) (not (not-flattire)))
    )
  )
  (:action changetire
    :parameters (?loc - location)
    :precondition (and (spare-in ?loc) (at ?loc))
    :effect (and (not (spare-in ?loc)) (not-flattire))
  )
)
