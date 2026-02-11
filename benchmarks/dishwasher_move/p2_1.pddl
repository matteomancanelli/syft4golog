(define (problem dishwasher_2_1)
  (:domain dishwasher)
  (:objects
    d1 - dish
    r1 r2 - room
  )
  (:init
    (robot-at r0)
    (emptyhand)
    (can-arrive d1)
  )
  (:goal (and
    (clean d1)
  ))
)