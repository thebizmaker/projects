import logging

from sqlalchemy.orm import Session

from app.database.models import PizzaType


def ingredients_are_available(pizza_type: PizzaType):
    if pizza_type.dough.stock == 0:
        logging.error('Not enough dough in stock for pizza with id {}'.format(pizza_type.id))
        return False

    if pizza_type.sauce.stock == 0:
        logging.error('Not enough sauce in stock for pizza with id {}'.format(pizza_type.id))
        return False

    for topping_quantity in pizza_type.toppings:
        if topping_quantity.topping.stock < topping_quantity.quantity:
            logging.error('Not enough {} for pizza with id {}'.format(topping_quantity.topping.name, pizza_type.id))
            return False

    return True


def reduce_stock_of_ingredients(pizza_type: PizzaType, db: Session):
    pizza_type.dough.stock -= 1
    pizza_type.sauce.stock -= 1

    for topping_quantity in pizza_type.toppings:
        topping_quantity.topping.stock -= topping_quantity.quantity

    db.commit()


def increase_stock_of_ingredients(pizza_type: PizzaType, db: Session):
    pizza_type.dough.stock += 1
    pizza_type.sauce.stock += 1

    for topping_quantity in pizza_type.toppings:
        topping_quantity.topping.stock += topping_quantity.quantity

    db.commit()
