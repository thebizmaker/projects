import pytest

import app.api.v1.endpoints.order.crud as order_crud
import app.api.v1.endpoints.user.crud as user_crud
import app.api.v1.endpoints.order.address.crud as address_crud
from app.api.v1.endpoints.order.schemas import OrderCreateSchema, OrderStatus
from app.api.v1.endpoints.user.schemas import UserCreateSchema
from app.api.v1.endpoints.order.address.schemas import AddressCreateSchema
from app.database.connection import SessionLocal


@pytest.fixture(scope='module')
def db():
    db = SessionLocal()
    try:
        yield db
    finally:
        db.close()


def test_order_create_read_update_delete(db):
    # Get initial counts
    number_of_users_before = len(user_crud.get_all_users(db))
    number_of_addresses_before = len(address_crud.get_all_addresses(db))
    number_of_orders_before = len(order_crud.get_all_orders(db))

    # Arrange: Instantiate a new user object
    user = UserCreateSchema(username='test_user')

    # Act: Add user to database
    created_user = user_crud.create_user(user, db)
    created_user_id = created_user.id

    # Assert: User created successfully
    users = user_crud.get_all_users(db)
    assert len(users) == number_of_users_before + 1

    # Arrange: Instantiate a new address object
    address = AddressCreateSchema(
        street='Test Street',
        post_code='12345',
        house_number=42,
        country='Test land',
        town='Test ville',
        first_name='Test',
        last_name='User',
    )

    # Act: Add address to database
    address_crud.create_address(address, db)

    # Assert: One more address in database
    addresses = address_crud.get_all_addresses(db)
    assert len(addresses) == number_of_addresses_before + 1

    # Arrange: Instantiate a new order object
    order_data = OrderCreateSchema(user_id=created_user_id, address=address)

    # Act: Add order to the database
    created_order = order_crud.create_order(order_data, db)
    created_order_id = created_order.id

    # Assert: One more order in database
    orders = order_crud.get_all_orders(db)
    assert len(orders) == number_of_orders_before + 1

    # Act: Re-read order from database
    read_order = order_crud.get_order_by_id(created_order_id, db)

    # Assert: Correct order was stored in database
    assert read_order is not None
    assert read_order.id == created_order_id
    assert read_order.user_id == created_user_id
    assert read_order.address.street == order_data.address.street
    assert read_order.order_status == OrderStatus.TRANSMITTED

    # Act: Update order status
    updated_order = order_crud.update_order_status(read_order, OrderStatus.PREPARING, db)

    # Assert: Correct status was stored in database
    assert updated_order.order_status == OrderStatus.PREPARING

    # Act: get orders by status
    orders_by_status = order_crud.get_orders_by_status(db, OrderStatus.PREPARING)

    # Assert: list is not empty
    assert len(orders_by_status) > 0

    # Assert all orders has the same status (PREPARING)
    for order in orders_by_status:
        assert order.order_status == OrderStatus.PREPARING

    # Act: Delete order
    order_crud.delete_order_by_id(created_order_id, db)

    # Assert: Correct number of addresses in database after deletion
    orders = order_crud.get_all_orders(db)
    assert len(orders) == number_of_orders_before

    # Assert: Correct order was deleted from database
    deleted_order = order_crud.get_order_by_id(created_order_id, db)
    assert deleted_order is None
