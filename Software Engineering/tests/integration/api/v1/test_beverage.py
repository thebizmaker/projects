import pytest

import app.api.v1.endpoints.beverage.crud as beverage_crud
from app.api.v1.endpoints.beverage.schemas import BeverageCreateSchema
from app.database.connection import SessionLocal


@pytest.fixture(scope='module')
def db():
    db = SessionLocal()
    try:
        yield db
    finally:
        db.close()


def test_create_read_update_delete(db):
    new_beverage_name = 'test'
    number_of_beverages_before = len(beverage_crud.get_all_beverages(db))

    # Arrange: Instantiate a new beverage object
    beverage = BeverageCreateSchema(name=new_beverage_name, price=5, description='test', stock=10)

    # Act: Add beverage to database
    db_beverage = beverage_crud.create_beverage(beverage, db)
    created_beverage_id = db_beverage.id

    # Assert: One more beverage in database
    beverages = beverage_crud.get_all_beverages(db)
    assert len(beverages) == number_of_beverages_before + 1

    # Act: Re-read beverage from database
    read_beverage = beverage_crud.get_beverage_by_id(created_beverage_id, db)
    read_beverage_by_name = beverage_crud.get_beverage_by_name(db_beverage.name, db)

    # Assert: Correct beverage was stored in database
    assert read_beverage.id == created_beverage_id
    assert read_beverage.name == new_beverage_name
    assert read_beverage_by_name.name == new_beverage_name
    assert read_beverage.price == 5
    assert read_beverage.description == 'test'
    assert read_beverage.stock == 10

    # Act: Update beverage
    updated_schema = BeverageCreateSchema(name='new_name', price=6, description='new_description', stock=9)
    beverage_crud.update_beverage(read_beverage, updated_schema, db)

    # Assert: Correct price and stock amount are stored in database
    assert read_beverage.name == 'new_name'
    assert read_beverage.price == 6
    assert read_beverage.description == 'new_description'
    assert read_beverage.stock == 9

    # Act: Delete beverage
    beverage_crud.delete_beverage_by_id(created_beverage_id, db)

    # Assert: Correct number of beverages in database after deletion
    beverages = beverage_crud.get_all_beverages(db)
    assert len(beverages) == number_of_beverages_before

    # Assert: Correct beverage was deleted from database
    deleted_beverage = beverage_crud.get_beverage_by_id(created_beverage_id, db)
    assert deleted_beverage is None
