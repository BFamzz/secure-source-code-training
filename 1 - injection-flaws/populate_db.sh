#!/bin/bash

set -e
PGPASSWORD="$POSTGRES_PASSWORD" psql -h postgres-db -p 5432 -U "$POSTGRES_USER" -d "$POSTGRES_DB" -f ./schema.sql
