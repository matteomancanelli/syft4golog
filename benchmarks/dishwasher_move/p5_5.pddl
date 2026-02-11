(define (problem dishwasher_5_5)
  (:domain dishwasher)
  (:objects
    d1 d2 d3 d4 d5 - dish
    r1 r2 r3 r4 r5 - room
  )
  (:init
    (robot-at r0)
    (emptyhand)
    (can-arrive d1)
    (can-arrive d2)
    (can-arrive d3)
    (can-arrive d4)
    (can-arrive d5)
  )
  (:goal (and
    (clean d1)
    (clean d2)
    (clean d3)
    (clean d4)
    (clean d5)
  ))
)