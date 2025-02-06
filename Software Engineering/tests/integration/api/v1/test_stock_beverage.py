import pytest

import app.api.v1.endpoints.beverage.crud as beverage_crud
from app.api.v1.endpoints.beverage.schemas import BeverageCreateSchema
from app.database.connection import SessionLocal
from app.api.v1.endpoints.order.stock_logic.stock_beverage_crud import beverage_is_available, change_stock_of_beverage


@pytest.fixture(scope='module')
def db():
    db = SessionLocal()
    try:
        yield db
    finally:
        db.close()


def test_stock_beverage_create_read_delete(db):
    new_beverage_name = 'test stock_beverage'
    new_beverage_price = 7.00
    new_beverage_description = 'test stock_beverage description'
    new_beverage_stock = 10
    number_of_beverages_before = len(beverage_crud.get_all_beverages(db))

    # Arrange: Instantiate a new beverage object
    beverage = BeverageCreateSchema(
        name=new_beverage_name,
        price=new_beverage_price,
        description=new_beverage_description,
        stock=new_beverage_stock,
    )

    # Act: Add beverage to database
    db_beverage = beverage_crud.create_beverage(beverage, db)
    created_beverage_id = db_beverage.id

    # Assert: One more beverage in database
    beverages = beverage_crud.get_all_beverages(db)
    assert len(beverages) == number_of_beverages_before + 1

    # Act: Re-read beverage from database
    read_beverage = beverage_crud.get_beverage_by_id(created_beverage_id, db)

    # Assert: Correct beverage was stored in database
    assert read_beverage.id == created_beverage_id
    assert read_beverage.name == new_beverage_name
    assert read_beverage.price == pytest.approx(new_beverage_price)
    assert read_beverage.description == new_beverage_description
    assert read_beverage.stock == new_beverage_stock

    # Beverage is available
    assert beverage_is_available(created_beverage_id, 10, db)
    assert not beverage_is_available(created_beverage_id, 11, db)

    # Change Stock of Beverage
    assert not change_stock_of_beverage(created_beverage_id, -11, db)
    assert change_stock_of_beverage(created_beverage_id, 5, db)

    # assert the stock was really changed
    assert read_beverage.stock == 15

    # Act: Delete beverage
    beverage_crud.delete_beverage_by_id(created_beverage_id, db)

    # Assert: Correct number of beverages in database after deletion
    beverages = beverage_crud.get_all_beverages(db)
    assert len(beverages) == number_of_beverages_before

    # Assert: Correct beverage was deleted from database
    deleted_beverage = beverage_crud.get_beverage_by_id(created_beverage_id, db)
    assert deleted_beverage is None
