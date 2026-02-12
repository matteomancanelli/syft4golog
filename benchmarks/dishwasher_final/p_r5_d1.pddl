(define (problem dishwasher_r5_d1)
(:domain warehouse-domain)
(:objects
  d1 - dish
  r1 r2 r3 r4 r5 - room
)
(:init
  (emptyhand)
  (robot-at r0)
  (dish-at d1 r1)
)
(:goal (and
  (dish-at d1 r0)
))
)