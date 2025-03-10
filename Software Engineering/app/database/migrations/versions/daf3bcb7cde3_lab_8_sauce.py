"""lab_8_sauce

Revision ID: daf3bcb7cde3
Revises: 39ae3140d696
Create Date: 2025-01-23 17:07:11.483751

"""
from alembic import op
import sqlalchemy as sa


# revision identifiers, used by Alembic.
revision = 'daf3bcb7cde3'
down_revision = '39ae3140d696'
branch_labels = None
depends_on = None


def upgrade():
    # ### commands auto generated by Alembic - please adjust! ###
    op.create_table('sauce',
    sa.Column('id', sa.Uuid(), nullable=False),
    sa.Column('name', sa.String(), nullable=False),
    sa.Column('price', sa.Numeric(precision=10, scale=2), nullable=False),
    sa.Column('description', sa.String(), nullable=False),
    sa.Column('stock', sa.Integer(), nullable=False),
    sa.PrimaryKeyConstraint('id'),
    sa.UniqueConstraint('name')
    )
    op.add_column('pizza_type', sa.Column('sauce_id', sa.Uuid(), nullable=False))
    op.create_foreign_key(None, 'pizza_type', 'sauce', ['sauce_id'], ['id'])
    # ### end Alembic commands ###


def downgrade():
    # ### commands auto generated by Alembic - please adjust! ###
    op.drop_constraint(None, 'pizza_type', type_='foreignkey')
    op.drop_column('pizza_type', 'sauce_id')
    op.drop_table('sauce')
    # ### end Alembic commands ###
