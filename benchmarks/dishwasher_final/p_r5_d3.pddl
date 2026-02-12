(define (problem dishwasher_r5_d3)
(:domain warehouse-domain)
(:objects
  d1 d2 d3 - dish
  r1 r2 r3 r4 r5 - room
)
(:init
  (emptyhand)
  (robot-at r0)
  (dish-at d1 r1)
  (dish-at d2 r2)
  (dish-at d3 r3)
)
(:goal (and
  (dish-at d1 r0)
  (dish-at d2 r0)
  (dish-at d3 r0)
))
)