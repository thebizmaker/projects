import pytest

import app.api.v1.endpoints.order.address.crud as address_crud
from app.api.v1.endpoints.order.address.schemas import AddressCreateSchema
from app.database.connection import SessionLocal


@pytest.fixture(scope='module')
def db():
    db = SessionLocal()
    try:
        yield db
    finally:
        db.close()


def test_address_create_read_delete(db):
    new_address_street = 'test address'
    new_address_post_code = '75001'
    new_address_house_number = 12
    new_address_country = 'test country'
    new_address_town = 'test town'
    new_address_first_name = 'test first name'
    new_address_last_name = 'test last name'
    number_of_addresses_before = len(address_crud.get_all_addresses(db))

    # Arrange: Instantiate a new address object
    address = AddressCreateSchema(
        street=new_address_street,
        post_code=new_address_post_code,
        house_number=new_address_house_number,
        country=new_address_country,
        town=new_address_town,
        first_name=new_address_first_name,
        last_name=new_address_last_name,
    )

    # Act: Add address to database
    db_address = address_crud.create_address(address, db)
    created_address_id = db_address.id

    # Assert: One more address in database
    addresses = address_crud.get_all_addresses(db)
    assert len(addresses) == number_of_addresses_before + 1

    # Act: Re-read address from database
    read_address = address_crud.get_address_by_id(created_address_id, db)

    # Assert: Correct address was stored in database
    assert read_address.id == created_address_id
    assert read_address.street == new_address_street
    assert read_address.post_code == new_address_post_code
    assert read_address.house_number == new_address_house_number
    assert read_address.country == new_address_country
    assert read_address.town == new_address_town
    assert read_address.first_name == new_address_first_name
    assert read_address.last_name == new_address_last_name

    # Act: Delete address
    address_crud.delete_address_by_id(created_address_id, db)

    # Assert: Correct number of addresses in database after deletion
    addresses = address_crud.get_all_addresses(db)
    assert len(addresses) == number_of_addresses_before

    # Assert: Correct address was deleted from database
    deleted_address = address_crud.get_address_by_id(created_address_id, db)
    assert deleted_address is None
