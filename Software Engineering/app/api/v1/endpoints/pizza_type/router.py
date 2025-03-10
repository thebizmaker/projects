import uuid
from typing import List, TypeVar
import logging

from fastapi import APIRouter, Depends, Request, Response, status, HTTPException
from fastapi.responses import RedirectResponse
from sqlalchemy.orm import Session

import app.api.v1.endpoints.dough.crud as dough_crud
import app.api.v1.endpoints.sauce.crud as sauce_crud
import app.api.v1.endpoints.pizza_type.crud as pizza_type_crud
import app.api.v1.endpoints.topping.crud as topping_crud
from app.api.v1.endpoints.dough.schemas import DoughSchema
from app.api.v1.endpoints.sauce.schemas import SauceSchema
from app.api.v1.endpoints.pizza_type.schemas import \
    JoinedPizzaTypeQuantitySchema, \
    PizzaTypeSchema, \
    PizzaTypeCreateSchema, \
    PizzaTypeToppingQuantityCreateSchema
from app.database.connection import SessionLocal
from app.constants import ITEM_NOT_FOUND

router = APIRouter()


def get_db():
    db = SessionLocal()
    try:
        yield db
    finally:
        db.close()


@router.get('', response_model=List[PizzaTypeSchema], tags=['pizza_type'])
def get_all_pizza_types(db: Session = Depends(get_db)):
    pizza_types = pizza_type_crud.get_all_pizza_types(db)
    return pizza_types


@router.post('', response_model=PizzaTypeSchema, tags=['pizza_type'])
def create_pizza_type(
        pizza_type: PizzaTypeCreateSchema, request: Request,
        response: Response, db: Session = Depends(get_db),
):
    pizza_type_found = pizza_type_crud.get_pizza_type_by_name(pizza_type.name, db)

    if pizza_type_found:
        url = request.url_for('get_pizza_type', pizza_type_id=pizza_type_found.id)
        logging.warning(f'Pizza type with the name {pizza_type_found.name} already exists')
        return RedirectResponse(url=url, status_code=status.HTTP_303_SEE_OTHER)

    dough = dough_crud.get_dough_by_id(pizza_type.dough_id, db)
    if not dough:
        logging.error(f'Dough with id {pizza_type.dough_id} for pizza type {pizza_type.name} not found')
        raise HTTPException(status_code=404, detail=ITEM_NOT_FOUND)

    sauce = sauce_crud.get_sauce_by_id(pizza_type.sauce_id, db)
    if not sauce:
        logging.error(f'Sauce with id {pizza_type.sauce_id} for pizza type {pizza_type.name} not found')
        raise HTTPException(status_code=404, detail=ITEM_NOT_FOUND)

    new_pizza_type = pizza_type_crud.create_pizza_type(pizza_type, db)
    response.status_code = status.HTTP_201_CREATED
    return new_pizza_type


@router.put('/{pizza_type_id}', response_model=PizzaTypeSchema, tags=['pizza_type'])
def update_pizza_type(
        pizza_type_id: uuid.UUID,
        changed_pizza_type: PizzaTypeCreateSchema,
        request: Request,
        response: Response,
        db: Session = Depends(get_db),
):
    pizza_type_found = pizza_type_crud.get_pizza_type_by_id(pizza_type_id, db)
    updated_pizza_type = None

    if pizza_type_found:
        if pizza_type_found.name == changed_pizza_type.name:
            pizza_type_crud.update_pizza_type(pizza_type_found, changed_pizza_type, db)
            logging.info(f'{pizza_type_found.name} is already the name of the pizza type')
            return Response(status_code=status.HTTP_204_NO_CONTENT)
        else:
            pizza_type_name_found = pizza_type_crud.get_pizza_type_by_name(changed_pizza_type.name, db)
            if pizza_type_name_found:
                url = request.url_for('get_pizza_type', pizza_type_id=pizza_type_name_found.id)
                logging.warning(f'Pizza type with the name {pizza_type_name_found.name} already exists')
                return RedirectResponse(url=url, status_code=status.HTTP_303_SEE_OTHER)
            else:
                updated_pizza_type = pizza_type_crud.create_pizza_type(changed_pizza_type, db)
                response.status_code = status.HTTP_201_CREATED
    else:
        logging.error(f'Pyzza type with id {pizza_type_id} not found')
        raise HTTPException(status_code=404, detail=ITEM_NOT_FOUND)

    return updated_pizza_type


@router.get('/{pizza_type_id}', response_model=PizzaTypeSchema, tags=['pizza_type'])
def get_pizza_type(
        pizza_type_id: uuid.UUID,
        db: Session = Depends(get_db),
):
    pizza_type = pizza_type_crud.get_pizza_type_by_id(pizza_type_id, db)

    if not pizza_type:
        logging.error(f'Pizza type with id {pizza_type_id} not found')
        raise HTTPException(status_code=404, detail=ITEM_NOT_FOUND)

    return pizza_type


@router.delete('/{pizza_type_id}', response_model=None, tags=['pizza_type'])
def delete_pizza_type(pizza_type_id: uuid.UUID,
                      db: Session = Depends(get_db),
                      ):
    pizza_type = pizza_type_crud.get_pizza_type_by_id(pizza_type_id, db)

    if not pizza_type:
        logging.error(f'Pizza type with id {pizza_type_id} not found')
        raise HTTPException(status_code=404, detail=ITEM_NOT_FOUND)

    pizza_type_crud.delete_pizza_type_by_id(pizza_type_id, db)
    logging.info(f'Pizza type with id {pizza_type_id} deleted')
    return Response(status_code=status.HTTP_204_NO_CONTENT)


# Due to mypy error, this workaround is needed for Union
# see pull request https://github.com/python/mypy/pull/8779
# should be fixed in near future
MyPyEitherItem = TypeVar(
    'MyPyEitherItem',
    List[PizzaTypeToppingQuantityCreateSchema],
    List[JoinedPizzaTypeQuantitySchema],
    None,
)


@router.get(
    '/{pizza_type_id}/toppings',
    response_model=MyPyEitherItem,
    tags=['pizza_type'],
)
def get_pizza_type_toppings(
        pizza_type_id: uuid.UUID,
        response: Response,
        db: Session = Depends(get_db),
        join: bool = False,
):
    pizza_type = pizza_type_crud.get_pizza_type_by_id(pizza_type_id, db)

    if not pizza_type:
        logging.error(f'Pizza type with id {pizza_type_id} not found')
        raise HTTPException(status_code=404, detail=ITEM_NOT_FOUND)

    toppings = pizza_type.toppings
    if join:
        toppings = pizza_type_crud.get_joined_topping_quantities_by_pizza_type(pizza_type.id, db)

    return toppings


@router.post(
    '/{pizza_type_id}/toppings',
    response_model=PizzaTypeToppingQuantityCreateSchema,
    status_code=status.HTTP_201_CREATED,
    tags=['pizza_type'],
)
def create_pizza_type_topping(
        pizza_type_id: uuid.UUID,
        topping_quantity: PizzaTypeToppingQuantityCreateSchema,
        request: Request,
        response: Response,
        db: Session = Depends(get_db),
):
    pizza_type = pizza_type_crud.get_pizza_type_by_id(pizza_type_id, db)
    if not pizza_type:
        logging.error(f'Pizza type with id {pizza_type_id} not found')
        raise HTTPException(status_code=404, detail=ITEM_NOT_FOUND)

    if not topping_crud.get_topping_by_id(topping_quantity.topping_id, db):
        logging.error(f'Topping with id {topping_quantity.topping_id} not found')
        raise HTTPException(status_code=404, detail=ITEM_NOT_FOUND)

    topping_quantity_found = pizza_type_crud.get_topping_quantity_by_id(pizza_type_id, topping_quantity.topping_id, db)
    if topping_quantity_found:
        url = request.url_for('get_pizza_type_toppings', pizza_type_id=topping_quantity_found.pizza_type_id)
        logging.warning(f'Topping quantity for pizza type {pizza_type.name} already exists')
        return RedirectResponse(url=url, status_code=status.HTTP_303_SEE_OTHER)

    new_topping_quantity = pizza_type_crud.create_topping_quantity(pizza_type, topping_quantity, db)
    return new_topping_quantity


@router.get(
    '/{pizza_type_id}/dough',
    response_model=DoughSchema,
    tags=['pizza_type'],
)
def get_pizza_type_dough(
        pizza_type_id: uuid.UUID,
        response: Response,
        db: Session = Depends(get_db),
):
    pizza_type = pizza_type_crud.get_pizza_type_by_id(pizza_type_id, db)

    if not pizza_type:
        logging.error(f'Pizza type with id {pizza_type_id} not found')
        raise HTTPException(status_code=404, detail=ITEM_NOT_FOUND)

    dough = pizza_type.dough

    return dough


@router.get(
    '/{pizza_type_id}/sauce',
    response_model=SauceSchema,
    tags=['pizza_type'],
)
def get_pizza_type_sauce(
        pizza_type_id: uuid.UUID,
        response: Response,
        db: Session = Depends(get_db),
):
    pizza_type = pizza_type_crud.get_pizza_type_by_id(pizza_type_id, db)

    if not pizza_type:
        logging.error(f'Pizza type with id {pizza_type_id} not found')
        raise HTTPException(status_code=404, detail=ITEM_NOT_FOUND)

    sauce = pizza_type.sauce

    return sauce
