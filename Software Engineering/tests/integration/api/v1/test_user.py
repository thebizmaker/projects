import pytest

import app.api.v1.endpoints.user.crud as user_crud
from app.api.v1.endpoints.user.schemas import UserCreateSchema
from app.database.connection import SessionLocal


@pytest.fixture(scope='module')
def db():
    db = SessionLocal()
    try:
        yield db
    finally:
        db.close()


def test_user_create_read_delete(db):
    new_user_name = 'test'
    number_of_users_before = len(user_crud.get_all_users(db))

    # Arrange: Instantiate a new user object
    user = UserCreateSchema(username=new_user_name)

    # Act: Add user to database
    db_user = user_crud.create_user(user, db)
    created_user_id = db_user.id

    # Assert: One more user in database
    users = user_crud.get_all_users(db)
    assert len(users) == number_of_users_before + 1

    # Act: Re-read user from database
    read_user = user_crud.get_user_by_id(created_user_id, db)

    # Assert: Correct user was stored in database
    assert read_user.id == created_user_id
    assert read_user.username == new_user_name

    # Act: Re-read user from database
    read_user_by_username = user_crud.get_user_by_username(db_user.username, db)

    # Assert: Correct user was stored in database
    assert read_user_by_username.id == created_user_id
    assert read_user_by_username.username == new_user_name

    # Arrange
    new_user = UserCreateSchema(username='test_2')

    # Act: Update User
    user_crud.update_user(db_user, new_user, db)

    # Assert
    assert db_user.username == new_user.username

    # Act: Delete user
    user_crud.delete_user_by_id(created_user_id, db)

    # Assert: Correct number of users in database after deletion
    users = user_crud.get_all_users(db)
    assert len(users) == number_of_users_before

    # Assert: Correct user was deleted from database
    deleted_user = user_crud.get_user_by_id(created_user_id, db)
    assert deleted_user is None
