import pytest

import app.api.v1.endpoints.pizza_type.crud as pizza_type_crud
import app.api.v1.endpoints.dough.crud as dough_crud
import app.api.v1.endpoints.sauce.crud as sauce_crud

from app.api.v1.endpoints.pizza_type.schemas import PizzaTypeCreateSchema
from app.api.v1.endpoints.dough.schemas import DoughCreateSchema
from app.api.v1.endpoints.sauce.schemas import SauceCreateSchema
from app.database.connection import SessionLocal
from decimal import Decimal


@pytest.fixture(scope='module')
def db():
    db = SessionLocal()
    try:
        yield db
    finally:
        db.close()


def create_dough():

    new_dough_name = 'test_dough_pizza_type'
    new_dough_price = Decimal('5.0')
    new_dough_description = 'test dough'
    new_dough_stock = 100

    dough = DoughCreateSchema(
        name=new_dough_name,
        price=new_dough_price,
        description=new_dough_description,
        stock=new_dough_stock,
    )

    return dough


def create_sauce():

    new_sauce_name = 'test_sauce_pizza_type'
    new_sauce_price = Decimal('5.0')
    new_sauce_description = 'test sauce'
    new_sauce_stock = 100

    sauce = SauceCreateSchema(
        name=new_sauce_name,
        price=new_sauce_price,
        description=new_sauce_description,
        stock=new_sauce_stock,
    )

    return sauce


def test_pizza_type_create_read_delete(db):
    new_pizza_type_name = 'test_pizza'
    new_pizza_type_price = Decimal('5.0')
    new_pizza_type_description = 'test pizza_type'
    number_of_pizza_types_before = len(pizza_type_crud.get_all_pizza_types(db))

    dough = create_dough()
    db_dough = dough_crud.create_dough(dough, db)

    sauce = create_sauce()
    db_sauce = sauce_crud.create_sauce(sauce, db)

    # Arrange: Instantiate a new pizza_type object
    pizza_type = PizzaTypeCreateSchema(
        name=new_pizza_type_name,
        price=new_pizza_type_price,
        description=new_pizza_type_description,
        dough_id=db_dough.id,
        sauce_id=db_sauce.id,
    )

    # Act: Add pizza_type to database
    db_pizza_type = pizza_type_crud.create_pizza_type(pizza_type, db)
    created_pizza_type_id = db_pizza_type.id
    created_pizza_type_name = db_pizza_type.name

    # Assert: One more pizza_type in database
    pizza_types = pizza_type_crud.get_all_pizza_types(db)
    assert len(pizza_types) == number_of_pizza_types_before + 1

    # Act: Re-read pizza_type from database
    read_pizza_type = pizza_type_crud.get_pizza_type_by_id(created_pizza_type_id, db)

    # Assert: Correct pizza_type was stored in database
    assert read_pizza_type.id == created_pizza_type_id
    assert read_pizza_type.name == new_pizza_type_name
    assert read_pizza_type.price == new_pizza_type_price
    assert read_pizza_type.description == new_pizza_type_description

    # Act: Re-read pizza_type from database
    read_pizza_type_by_name = pizza_type_crud.get_pizza_type_by_name(created_pizza_type_name, db)

    # Assert: Correct pizza_type was stored in database
    assert read_pizza_type_by_name.id == created_pizza_type_id
    assert read_pizza_type_by_name.name == new_pizza_type_name
    assert read_pizza_type_by_name.price == new_pizza_type_price
    assert read_pizza_type_by_name.description == new_pizza_type_description

    # Act: Delete pizza_type, dough and sauce
    pizza_type_crud.delete_pizza_type_by_id(created_pizza_type_id, db)
    dough_crud.delete_dough_by_id(db_dough.id, db)
    sauce_crud.delete_sauce_by_id(db_sauce.id, db)

    # Assert: Correct number of pizza_types in database after deletion
    pizza_types = pizza_type_crud.get_all_pizza_types(db)
    assert len(pizza_types) == number_of_pizza_types_before

    # Assert: Correct pizza_type was deleted from database
    deleted_pizza_type = pizza_type_crud.get_pizza_type_by_id(created_pizza_type_id, db)
    deleted_dough = dough_crud.get_dough_by_id(db_dough.id, db)
    deleted_sauce = sauce_crud.get_sauce_by_id(db_sauce.id, db)

    assert deleted_pizza_type is None
    assert deleted_dough is None
    assert deleted_sauce is None
