ALTER TABLE schneider.node ADD COLUMN "numId" integer;
ALTER TABLE schneider.node ALTER COLUMN "numId" SET NOT NULL;
ALTER TABLE schneider.node ALTER COLUMN "numId" SET DEFAULT nextval('schneider."node_numId_seq"'::regclass);