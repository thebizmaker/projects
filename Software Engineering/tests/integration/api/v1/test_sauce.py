import pytest

import app.api.v1.endpoints.sauce.crud as sauce_crud
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


def test_sauce_create_read_delete(db):
    new_sauce_name = 'test_sauce'
    new_sauce_price = Decimal('1.0')
    new_sauce_description = 'test sauce'
    new_sauce_stock = 100
    number_of_sauces_before = len(sauce_crud.get_all_sauces(db))

    # Arrange: Instantiate a new sauce object
    sauce = SauceCreateSchema(
        name=new_sauce_name,
        price=new_sauce_price,
        description=new_sauce_description,
        stock=new_sauce_stock,
    )

    # Act: Add sauce to database
    db_sauce = sauce_crud.create_sauce(sauce, db)
    created_sauce_id = db_sauce.id

    # Assert: One more sauce in database
    sauces = sauce_crud.get_all_sauces(db)
    assert len(sauces) == number_of_sauces_before + 1

    # Act: Re-read sauce from database
    read_sauce = sauce_crud.get_sauce_by_id(created_sauce_id, db)

    # Assert: Correct sauce was stored in database
    assert read_sauce.id == created_sauce_id
    assert read_sauce.name == new_sauce_name
    assert read_sauce.price == new_sauce_price
    assert read_sauce.description == new_sauce_description

    # Act: Delete sauce
    sauce_crud.delete_sauce_by_id(created_sauce_id, db)

    # Assert: Correct number of sauces in database after deletion
    sauces = sauce_crud.get_all_sauces(db)
    assert len(sauces) == number_of_sauces_before

    # Assert: Correct sauce was deleted from database
    deleted_sauce = sauce_crud.get_sauce_by_id(created_sauce_id, db)
    assert deleted_sauce is None
